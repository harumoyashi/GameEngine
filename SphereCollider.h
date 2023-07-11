#pragma once
#include "NBaseCollider.h"
#include "NCollider.h"

class SphereCollider:
	public NBaseCollider,public Sphere
{
private:
	NVector3 offset_;	//オブジェクト中心からのオフセット
	float radius_;		//半径

public:
	SphereCollider(NVector3 offset = NVector3(0, 0, 0), float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//形状設定
	}

	void Update()override;

	inline void SetRadius(float radius) { radius_ = radius; }
};