#include "Bullet.h"

Bullet::Bullet() :
	moveAngle(0.0f), moveSpeed(2.0f), isActive(true),
	collisionRadius(1.0f), damage(1), isCanGenerate(false)
{
}

void Bullet::Generate(const NVector3& pos)
{
	obj = move(std::make_unique<NObj3d>());
	obj->SetModel("Sphere");
	obj->position_ = pos;
	obj->scale_ = 0.5f;
	collider.centerPos = obj->position_;
	collider.radius = collisionRadius;
}

void Bullet::Update()
{
	//コライダーの設定
	collider.centerPos = obj->position_;
	collider.radius = collisionRadius;

	obj->Update();
}

void Bullet::Draw()
{
	obj->Draw();
}