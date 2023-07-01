#include "IBullet.h"

IBullet::IBullet() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(1.0f),
	isAlive_(true), collisionRadius_(1.0f), lifeTimer_(120.0f), damage_(1)
{
}

void IBullet::Generate(const NVector3& pos, const float moveAngle)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("Sphere");
	obj_->position_ = pos;
	obj_->scale_ = 0.5f;
	obj_->color_ = NColor::kWhite;
	collider_.centerPos = obj_->position_;
	collider_.radius = collisionRadius_;

	moveAngle_ = moveAngle;
}

void IBullet::Update()
{
	//生存時間過ぎたら死んだ判定に
	lifeTimer_.Update();
	if (lifeTimer_.GetisTimeOut())
	{
		isAlive_ = false;
	}

	//弾の移動
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_;

	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	//コライダーの設定
	collider_.centerPos = obj_->position_;
	collider_.radius = collisionRadius_;

	obj_->Update();
}

void IBullet::Draw()
{
	obj_->Draw();
}