#include "IEnemy.h"
#include "Player.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"

//スピードは基本プレイヤーよりちょい遅め
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_)
{
}

IEnemy::~IEnemy()
{
}

void IEnemy::Generate(const NVector3& pos, const float moveAngle, const std::string& modelname)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel(modelname);

	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 1.5f;
	obj_->color_ = NColor::kWhite;
	obj_->Update();

	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("enemy");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IEnemy::OnCollision, this));

	moveAngle_ = moveAngle;
}

void IEnemy::Init()
{
}

void IEnemy::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//移動
	Move();

	//移動に応じて回転
	//NVector2 velo = moveVelo_;	//moveVelo_の値が変わらないように格納
	//velo.Normalize();
	//moveAngle_ = MathUtil::Radian2Degree(acosf(velo.Dot({ 0,1 })));
	//if (velo.x < 0)
	//{
	//	moveAngle_ = -moveAngle_;
	//}

	obj_->rotation_.y = moveAngle_;

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void IEnemy::Draw()
{
	if (isAlive_)
	{
		obj_->Draw();
	}
}

void IEnemy::OnCollision()
{
	//当たった相手が弾だった時の処理
	if (collider_.GetColInfo()->GetColID() == "bullet")
	{
		DeadParticle();
		isAlive_ = false;
		NAudioManager::Play("vanishSE");
	}
}

void IEnemy::AddEmitter(uint32_t eneNum)
{
	//識別番号をつける
	enemyNum_ = eneNum;
	//パーティクルエミッターをマネージャーに登録
	NParticleManager::GetInstance()->enemyEmitters_.emplace_back();
	NParticleManager::GetInstance()->enemyEmitters_.back() = &deadParticle_;

}

void IEnemy::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetIsRotation(true);
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetPos(GetPos());
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->Add(
			20, 15, NColor::kLightblue, 0.1f, 0.4f, { -0.5f,-0.5f,-0.5f }, { 0.5f,0.5f,0.5f }, { 0,0,0 }, { -1,-1,-1 }, { 1,1,1 });
	}
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;
}
