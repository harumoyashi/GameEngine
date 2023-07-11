#pragma once
#include "NVector3.h"

class NObj3d;
class NBaseCollider;

//衝突情報
class NCollisionInfo
{
public:
	NCollisionInfo(NBaseCollider* collder, const NVector3& inter):
		collider_(collder),inter_(inter)
	{
	}

	NBaseCollider* collider_;	//衝突相手のコライダー
	NVector3 inter_;			//衝突点
};