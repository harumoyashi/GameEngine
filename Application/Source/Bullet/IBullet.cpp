#include "IBullet.h"
#include "Player.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"

IBullet::IBullet() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.2f), isAlive_(true),
	lifeTimer_(2.0f), damage_(1), elapseSpeed_(0.0f)
{
}

IBullet::~IBullet()
{
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

void IBullet::Generate(const NVec3& pos, const float moveAngle)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("sphere");
	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 0.7f;
	obj_->color_ = Player::GetInstance()->GetColor();

	//コライダー設定
	collider_.SetCenterPos({ obj_->position_.x,obj_->position_.z });
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("bullet");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IBullet::OnCollision, this));

	moveAngle_ = moveAngle;

	//エミッターの設定
	NParticleManager::GetInstance()->bulletEmitters_[bulletNum_]->SetIsRotation(true);
	NParticleManager::GetInstance()->bulletEmitters_[bulletNum_]->SetIsGrowing(true);
}

void IBullet::Update()
{
	//経過時間を適用
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//生存時間過ぎたら死んだ判定に
	lifeTimer_.Update(elapseSpeed_);
	if (lifeTimer_.GetStarted() == false)
	{
		lifeTimer_.Start();
	}
	else if (lifeTimer_.GetEnd())
	{
		isAlive_ = false;
	}

	//弾の移動
	//移動量 = 進む方向 * スピード * 経過時間
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()で呼ぶと、そのフレームでの総当たりに影響が出るからここで消してる
	if (isAlive_ == false)
	{
		//コライダーマネージャーから削除
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void IBullet::Draw()
{
	if (isAlive_)
	{
		obj_->SetBlendMode(BlendMode::Alpha);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::Alpha);
	}
}

void IBullet::OnCollision()
{
	//当たった相手が弾だった時の処理
	if (collider_.GetColInfo()->GetColID() == "enemy" || collider_.GetColInfo()->GetColID() == "boss")
	{
		DeadParticle();
		isAlive_ = false;
		collider_.SetIsActive(false);
	}
}

void IBullet::AddEmitter(uint32_t bulNum)
{
	//識別番号をつける
	bulletNum_ = bulNum;
	//パーティクルエミッターをマネージャーに登録
	NParticleManager::GetInstance()->bulletEmitters_.emplace_back();
	NParticleManager::GetInstance()->bulletEmitters_.back() = &deadParticle_;
}

void IBullet::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->bulletEmitters_[bulletNum_]->SetPos(GetPos());
		NParticleManager::GetInstance()->bulletEmitters_[bulletNum_]->Add(
			5, 0.25f, obj_->color_, 0.1f, 0.2f, { -0.3f,-0.1f,-0.3f }, { 0.3f,0.1f,0.3f }, 0.05f, -NVec3::one, NVec3::one);
	}
}
