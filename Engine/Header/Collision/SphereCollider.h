#pragma once
#include "NBaseCollider.h"

class SphereCollider final :
	public NBaseCollider
{
private:
	NVec3 centerPos_;	//コライダーの中心座標
	NVec3 offset_;		//オブジェクト中心からのオフセット
	float radius_;		//半径

public:
	SphereCollider(NVec3 offset = NVec3::zero, float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//形状設定
	}

	void Update(NObj3d* obj)override;

	// ゲッター //
	//半径取得
	float GetRadius()const { return radius_; }
	//コライダーの中心座標取得
	const NVec3& GetCenterPos()const { return centerPos_; }

	// セッター //
	//半径設定
	void SetRadius(float radius) { radius_ = radius; }
	//コライダーの中心座標設定
	void SetCenterPos(const NVec3& centerPos) { centerPos_ = centerPos; }
};