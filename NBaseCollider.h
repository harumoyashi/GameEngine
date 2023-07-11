#pragma once
#include "NObj3d.h"
#include "NCollisionInfo.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//未定義
		COL_SPHERE,				//球
	};

protected:
	NObj3d* obj_ = nullptr;					//オブジェクト
	ColShapeType shapeType = SHAPE_UNKNOWN;	//形状タイプ

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update() = 0;

	inline void OnCollision(const NCollisionInfo& info)
	{
		obj_->OnCollision(info);
	}

	// ゲッター //
	//オブジェクト取得
	inline NObj3d* GetObj3d() { return obj_; }
	//形状タイプ取得
	inline ColShapeType GetShapeType() { return shapeType; }

	// セッター //
	//オブジェクト設定
	inline void SetObj(NObj3d* obj) { obj_ = obj; }
};

