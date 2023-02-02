#include "NCollision.h"

bool NCollision::SphereCol(const Sphere& s0, const Sphere& s1)
{
	float distX = (s0.pos.x - s1.pos.x) * (s0.pos.x - s1.pos.x);
	float distY = (s0.pos.y - s1.pos.y) * (s0.pos.y - s1.pos.y);
	float distZ = (s0.pos.z - s1.pos.z) * (s0.pos.z - s1.pos.z);
	float distR = (s0.radius + s1.radius) * (s0.radius + s1.radius);

	if (distX + distY + distZ <= distR)
	{
		return true;
	}
	return false;
}

bool NCollision::Sphere2PlaneCol(const Sphere& sphere, const Plane& plane/*, NVector3 inter*/)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = sphere.pos.Dot(plane.normal);

	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.distance;

	//�����̐�Βl�����a���傫����Γ������ĂȂ�
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	return true;
}