#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#include "NVector2.h"
#include "NVector3.h"

class NCollision
{
public:
	struct Sphere
	{
		NVector3 pos = { 0.0f,0.0f,0.0f };	//���W
		float radius = 1.0f;	//���̔��a
	};

	struct Plane
	{
		NVector3 normal = { 0.0f,1.0f,0.0f };	//�@��
		float distance = 0.0f;	//���_����̋���
	};

	//�����m�̓����蔻��
	//col:�����蔻��Q�Ƃ�����
	static bool SphereCol(const Sphere& s0, const Sphere& s1);

	static bool Sphere2PlaneCol(const Sphere& sphere,const Plane& plane/*,NVector3 inter*/);
};