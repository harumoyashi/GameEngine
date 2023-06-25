#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#include "NCollider.h"

class NCollision
{
public:
	//�~���m�̓����蔻��
	static bool CircleCol(const CircleCollider& c0, const CircleCollider& c1);

	//�����m�̓����蔻��
	static bool SphereCol(const SphereCollider& s0, const SphereCollider& s1);
	
	//���ƕ��ʂ̓����蔻��
	static bool Sphere2PlaneCol(const SphereCollider& sphere,const PlaneCollider& plane/*,NVector3 inter*/);
};