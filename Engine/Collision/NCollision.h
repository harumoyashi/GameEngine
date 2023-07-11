#pragma once
#include <DirectXMath.h>

#include "NCollider.h"

class NCollision
{
public:
	//�~���m�̓����蔻��
	static bool CircleCol(const Circle& c0, const Circle& c1,const NVector2& inter);

	//�����m�̓����蔻��
	static bool SphereCol(const Sphere& s0, const Sphere& s1, const NVector3& inter);
	
	//���ƕ��ʂ̓����蔻��
	static bool Sphere2PlaneCol(const Sphere& sphere,const Plane& plane,NVector3 inter);
};