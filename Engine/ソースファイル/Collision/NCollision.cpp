#include "NCollision.h"

bool NCollision::CircleCol(const Circle& c0, const Circle& c1, const NVec2& inter)
{
	float distX = (c0.centerPos.x - c1.centerPos.x) * (c0.centerPos.x - c1.centerPos.x);
	float distY = (c0.centerPos.y - c1.centerPos.y) * (c0.centerPos.y - c1.centerPos.y);
	float distR = (c0.radius + c1.radius) * (c0.radius + c1.radius);

	if (distX + distY <= distR)
	{
		return true;
	}
	return false;
}

bool NCollision::SphereCol(const SphereCollider& s0, const SphereCollider& s1, const NVec3& inter)
{
	float distX = (s0.GetCenterPos().x - s1.GetCenterPos().x) * (s0.GetCenterPos().x - s1.GetCenterPos().x);
	float distY = (s0.GetCenterPos().y - s1.GetCenterPos().y) * (s0.GetCenterPos().y - s1.GetCenterPos().y);
	float distZ = (s0.GetCenterPos().z - s1.GetCenterPos().z) * (s0.GetCenterPos().z - s1.GetCenterPos().z);
	float distR = (s0.GetRadius() + s1.GetRadius()) * (s0.GetRadius() + s1.GetRadius());

	if (distX + distY + distZ <= distR)
	{
		return true;
	}
	return false;
}

bool NCollision::Sphere2PlaneCol(const SphereCollider& sphere, const PlaneCollider& plane)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = sphere.GetCenterPos().Dot(plane.GetNormal());

	//���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.GetDistance();

	//�����̐�Βl�����a���傫����Γ������ĂȂ�
	if (fabsf(dist) > sphere.GetRadius())
	{
		return false;
	}

	return true;
}

bool NCollision::Ray2PlaneCol(const RayCollider& ray, const PlaneCollider& plane)
{
	const float epsilon = 1.0e-5f;	//�덷�z���p�̋ɏ��̒l
	//�ʖ@���ƃ��C�̕����x�N�g���̓���
	float d1 = plane.GetNormal().Dot(ray.GetDirVec());
	//���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon)
	{
		return false;
	}
	//�n�_�ƌ��_�̋���(���ʂ̖@������)
	//�ʖ@���ƃ��C�̎n�_���W(�ʒu�x�N�g��)�̓���
	float d2 = plane.GetNormal().Dot(ray.GetStartPos());
	//�n�_�ƕ��ʂ̋���(���ʂ̖@������)
	float dist = d2 - plane.GetDistance();
	//�n�_�ƕ��ʂ̋���(���C����)
	float t = dist / -d1;
	//��_���n�_�����ɂ���̂ŁA������Ȃ�
	if (t < 0)
	{
		return false;
	}
	//��_���v�Z
	

	return true;
}
