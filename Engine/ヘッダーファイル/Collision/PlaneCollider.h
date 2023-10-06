#pragma once
#include "NBaseCollider.h"

class PlaneCollider final :
	public NBaseCollider
{
private:
	NVec3 normal_;		//法線
	float distance_;	//原点からの距離

public:
	PlaneCollider(NVec3 normal = NVec3::up, float distance = 0.f) :
		normal_(normal), distance_(distance)
	{
		shapeType_ = COL_PLANE;
	}

	void Update(NObj3d* obj)override;

	// ゲッター //
	//原点からの距離取得
	float GetDistance()const { return distance_; }
	//法線取得
	const NVec3& GetNormal()const { return normal_; }

	// セッター //
	//原点からの距離設定
	void SetDistance(float distance) { distance_ = distance; }
	//法線設定
	void SetNormal(const NVec3& normal) { normal_ = normal; }
};

