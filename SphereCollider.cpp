#include "SphereCollider.h"

void SphereCollider::Update()
{
	Sphere::centerPos = NBaseCollider::obj_->position_ + offset_;
	Sphere::radius = radius;
}
