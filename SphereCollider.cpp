#include "SphereCollider.h"

void SphereCollider::Update()
{
	Sphere::centerPos = NBaseCollider::obj_->GetMatWorld().GetWorldPos() + offset_;
	Sphere::radius = SphereCollider::radius_;
}