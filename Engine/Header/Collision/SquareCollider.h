#pragma once
#include "NBaseCollider.h"

// 矩形のコライダー //

class SquareCollider :
    public NBaseCollider
{
private:
	NVec2 centerPos_;	//コライダーの中心座標
	NVec2 offset_;		//オブジェクト中心からのオフセット
	float wide_;		//横幅(比率)
	float height_;		//縦幅(比率)
	float size_;		//大きさ

public:
	SquareCollider(NVec2 offset = { 0.f,0.f }, float wide = 1.0f, float height = 1.0f) :
		offset_(offset), wide_(wide), height_(height)
	{
		shapeType_ = COL_SQUARE;	//形状設定
	}

	void Update(NObj3d* obj)override;

	// ゲッター //
	//横幅取得
	float GetWide()const { return wide_; }
	//縦幅取得
	float GetHeight()const { return height_; }
	//大きさ取得
	float GetSize()const { return size_; }
	//コライダーの中心座標取得
	const NVec2& GetCenterPos()const { return centerPos_; }

	// セッター //
	//横幅設定
	void SetWide(float wide) { wide_ = wide; }
	//縦幅設定
	void SetHeight(float height) { height_ = height; }
	//大きさ設定
	void SetSize(float size) { size_ = size; }
	//コライダーの中心座標設定
	void SetCenterPos(const NVec2& centerPos) { centerPos_ = centerPos; }
};