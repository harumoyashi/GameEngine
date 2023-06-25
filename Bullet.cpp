#include "Bullet.h"

Bullet::Bullet() :
	moveAngle_(0.0f), moveSpeed_(2.0f), isActive_(true),
	collisionRadius_(1.0f), damage_(1), isCanGenerate_(false)
{
}

void Bullet::Generate(const NVector3& pos)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("Sphere");
	obj_->position_ = pos;
	obj_->scale_ = 0.5f;
	collider_.centerPos = obj_->position_;
	collider_.radius = collisionRadius_;
}

void Bullet::Update()
{
	//コライダーの設定
	collider_.centerPos = obj_->position_;
	collider_.radius = collisionRadius_;

	obj_->Update();
}

void Bullet::Draw()
{
	obj_->Draw();
}