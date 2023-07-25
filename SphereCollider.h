#pragma once
#include "NBaseCollider.h"
#include "NCollider.h"

class SphereCollider final :
	public NBaseCollider
{
private:
	NVector3 centerPos_;	//コライダーの中心座標
	NVector3 offset_;		//オブジェクト中心からのオフセット
	float radius_;			//半径

public:
	SphereCollider(NVector3 offset = NVector3(0, 0, 0), float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//形状設定
	}

	void Update(NObj3d* obj)override;

	// ゲッター //
	//半径設定
	float GetRadius()const { return radius_; }
	//コライダーの中心座標取得
	const NVector3& GetCenterPos()const { return centerPos_; }

	// セッター //
	//半径設定
	void SetRadius(float radius) { radius_ = radius; }
	//コライダーの中心座標設定
	void SetCenterPos(NVector3 centerPos) { centerPos_ = centerPos; }
};