#pragma once
#include "NVector3.h"

class NObj3d;
class NBaseCollider;

//�Փˏ��
class NCollisionInfo
{
public:
	NCollisionInfo(NBaseCollider* collder, const NVector3& inter):
		collider_(collder),inter_(inter)
	{
	}

	NBaseCollider* collider_;	//�Փˑ���̃R���C�_�[
	NVector3 inter_;			//�Փ˓_
};