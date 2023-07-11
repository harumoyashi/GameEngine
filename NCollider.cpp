#include "NCollider.h"

Circle::Circle() :
	centerPos(0, 0), radius(1)
{
}

Circle::Circle(const NVector2& centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
}

Sphere::Sphere() :
	centerPos(0, 0, 0), radius(1)
{
}

Sphere::Sphere(const NVector3& centerPos, const float radius) :
	centerPos(centerPos), radius(radius)
{
}

void Sphere::Init()
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("sphere");
	
	obj_->position_ = centerPos;
	obj_->scale_ = { radius,radius,radius };
}

void Sphere::Update()
{
	obj_->position_ = centerPos;
	obj_->Update();
}

void Sphere::Draw()
{
	obj_->Draw();
}

Ray::Ray() :
	startPos(0, 0, 0), dirVec(0, 0, 1)
{
}

Ray::Ray(const NVector3& startPos, const NVector3& dirVec) :
	startPos(startPos), dirVec(dirVec)
{
}

Line::Line() :
	startPos(0, 0, 0), endPos(0, 0, 0)
{
}

Line::Line(const NVector3& startPos, const NVector3& endPos) :
	startPos(startPos), endPos(endPos)
{
}

Plane::Plane() :
	normal(0, 1, 0), distance(0)
{
}

Plane::Plane(const NVector3& normal, const float distance) :
	normal(normal), distance(distance)
{
}
