#pragma once
#include "NObj3d.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//未定義
		COL_SPHERE,				//球
	};

protected:
	NObj3d* obj_ = nullptr;						//オブジェクト
	ColShapeType shapeType_ = SHAPE_UNKNOWN;	//形状タイプ
	bool isCollision_;							//当たったかフラグ

	NBaseCollider* colInfo_;					//衝突相手のコライダー
	std::string colID;							//コライダーの識別ID
	NVector3 inter_;							//衝突点

	std::function<void(void)>onCollision_;		//コールバック関数ポインタ変数

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update(NObj3d* obj) = 0;

	// ゲッター //
	//オブジェクト取得
	inline NObj3d* GetObj3d() { return obj_; }
	//形状タイプ取得
	inline ColShapeType GetShapeType() { return shapeType_; }
	//当たってるかフラグ取得
	inline bool GetIsCol() { return isCollision_; }
	//コライダー自身を取得
	inline NBaseCollider* GetCollider() { return this; }

public:
	// ゲッター //
	//コールバック関数ポインタ変数取得
	inline std::function<void(void)> GetOnCollision()const { return onCollision_; }
	//コライダーの識別ID取得
	inline std::string GetColID()const { return colID; }
	//衝突相手の取得
	inline NBaseCollider* GetColInfo()const { return colInfo_; }

	// セッター //
	//オブジェクト設定
	inline void SetObj(NObj3d* obj) { obj_ = obj; }
	//当たってるかフラグ設定
	inline void SetIsCol(bool isCol) { isCollision_ = isCol; }
	//コールバック関数ポインタ変数設定
	inline void SetOnCollision(const std::function<void(void)>& onCollision) { onCollision_ = onCollision; }
	//コライダーの識別ID設定
	inline void SetColID(std::string id) { colID = id; }
	//衝突相手の設定
	inline void SetColInfo(NBaseCollider* colInfo) { colInfo_ = colInfo; }
};