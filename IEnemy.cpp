#include "IEnemy.h"
#include "Player.h"
#include "SphereCollider.h"

//スピードは基本プレイヤーよりちょい遅め
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	colliderRadius_(1.0f), elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_)
{
}

void IEnemy::Generate(const NVector3& pos, const float moveAngle, const std::string& modelname)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel(modelname);

	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 1.5f;
	obj_->color_ = NColor::kWhite;

	colliderRadius_ = obj_->scale_.x;
	collider_.centerPos = obj_->position_;
	collider_.radius = colliderRadius_;
	obj_->SetCollider(new SphereCollider(obj_->position_, colliderRadius_));

	moveAngle_ = moveAngle;

	collider_.Init();
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

	//コライダーの設定
	collider_.centerPos = obj_->position_;
	collider_.radius = colliderRadius_;

	collider_.Update();
	obj_->Update();
}

void IEnemy::Draw()
{
	collider_.Draw();
	obj_->Draw();
}

void IEnemy::OnCollision(const NCollisionInfo& info)
{
	collider_.obj_->color_ = NColor::kPink;
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//移動
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;
}
