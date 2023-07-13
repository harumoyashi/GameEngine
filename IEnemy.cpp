#include "IEnemy.h"
#include "Player.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"

//スピードは基本プレイヤーよりちょい遅め
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_)
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
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IEnemy::OnCollision, this));

	moveAngle_ = moveAngle;
}

bool IEnemy::Init()
{
	return true;
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
}

void IEnemy::Draw()
{
	obj_->Draw();
}

void IEnemy::OnCollision()
{
	obj_->color_ = NColor::kRed;
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;
}
