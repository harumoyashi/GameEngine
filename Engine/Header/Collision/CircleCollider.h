#pragma once
#include "NBaseCollider.h"

// 円形のコライダー //

class CircleCollider :
    public NBaseCollider
{
private:
	NVec2 centerPos_;	//コライダーの中心座標
	NVec2 offset_;		//オブジェクト中心からのオフセット
	float radius_;		//半径

public:
	//コンストラクタ
	CircleCollider(NVec2 offset = {0.f,0.f}, float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_CIRCLE;	//形状設定
	}

	//更新
	void Update(NObj3d* obj)override;

	// ゲッター //
	//半径取得
	float GetRadius()const { return radius_; }
	//コライダーの中心座標取得
	const NVec2& GetCenterPos()const { return centerPos_; }

	// セッター //
	//半径設定
	void SetRadius(float radius) { radius_ = radius; }
	//コライダーの中心座標設定
	void SetCenterPos(const NVec2& centerPos) { centerPos_ = centerPos; }
};