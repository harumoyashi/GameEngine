#include "NCollider.h"

CircleCollider::CircleCollider() :
	centerPos(0, 0), radius(1)
{
}

CircleCollider::CircleCollider(const NVector2& centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
}

SphereCollider::SphereCollider() :
	centerPos(0, 0, 0), radius(1)
{
}

SphereCollider::SphereCollider(const NVector3& centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
}

void SphereCollider::Init()
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("sphere");
	
	obj_->position_ = centerPos;
	obj_->scale_ = { radius,radius,radius };
}

void SphereCollider::Update()
{
	obj_->position_ = centerPos;
	obj_->Update();
}

void SphereCollider::Draw()
{
	obj_->Draw();
}

ReyCollider::ReyCollider() :
	startPos(0, 0, 0), dirVec(0, 0, 1)
{
}

ReyCollider::ReyCollider(const NVector3& startPos, const NVector3& dirVec) :
	startPos(startPos), dirVec(dirVec)
{
}

LineCollider::LineCollider() :
	startPos(0, 0, 0), endPos(0, 0, 0)
{
}

LineCollider::LineCollider(const NVector3& startPos, const NVector3& endPos) :
	startPos(startPos), endPos(endPos)
{
}

PlaneCollider::PlaneCollider() :
	normal(0, 1, 0), distance(0)
{
}

PlaneCollider::PlaneCollider(const NVector3& normal, const float distance) :
	normal(normal), distance(distance)
{
}
