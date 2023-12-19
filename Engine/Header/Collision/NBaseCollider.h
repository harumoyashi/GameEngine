#pragma once
#include "NObj3d.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//未定義
		COL_SPHERE,				//球
		COL_PLANE,				//平面
		COL_RAY,				//レイ
		COL_CIRCLE,				//円
		COL_SQUARE,				//矩形
	};

	//衝突属性定義
	const unsigned short COL_TYPE_FIELD = 0b1 << 0;		//地形
	const unsigned short COL_TYPE_PLAYER = 0b1 << 1;	//プレイヤーまわり
	const unsigned short COL_TYPE_ENEMY = 0b1 << 2;		//敵まわり
	const unsigned short COL_TYPE_OBJECT = 0b1 << 3;	//オブジェクトまわり

protected:
	NObj3d* obj_ = nullptr;							//オブジェクト
	ColShapeType shapeType_ = SHAPE_UNKNOWN;		//形状タイプ
	bool isCollision_;								//当たったかフラグ

	NBaseCollider* colInfo_;						//衝突相手のコライダー
	std::string colID;								//コライダーの識別ID
	NVec3 inter_;									//衝突点
	unsigned short colType_ = 0b0000000000000000;	//衝突属性

	std::function<void(void)>onCollision_;			//コールバック関数ポインタ変数

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update(NObj3d* obj) = 0;

	// ゲッター //
	//オブジェクト取得
	NObj3d* GetObj3d() { return obj_; }
	//形状タイプ取得
	ColShapeType GetShapeType() { return shapeType_; }
	//当たってるかフラグ取得
	bool GetIsCol() { return isCollision_; }
	//コライダー自身を取得
	NBaseCollider* GetCollider() { return this; }

public:
	// ゲッター //
	//コールバック関数ポインタ変数取得
	std::function<void(void)> GetOnCollision()const { return onCollision_; }
	//コライダーの識別ID取得
	std::string GetColID()const { return colID; }
	//衝突相手の取得
	NBaseCollider* GetColInfo()const { return colInfo_; }

	// セッター //
	//オブジェクト設定
	void SetObj(NObj3d* obj) { obj_ = obj; }
	//当たってるかフラグ設定
	void SetIsCol(bool isCol) { isCollision_ = isCol; }
	//コールバック関数ポインタ変数設定
	void SetOnCollision(const std::function<void(void)>& onCollision) { onCollision_ = onCollision; }
	//コライダーの識別ID設定
	void SetColID(std::string id) { colID = id; }
	//衝突相手の設定
	void SetColInfo(NBaseCollider* colInfo) { colInfo_ = colInfo; }
	//衝突属性の設定
	void SetColType(unsigned short colType) { colType_ = colType; }
	//衝突属性の追加
	void AddColType(unsigned short colType) { colType_ |= colType; }
	//衝突属性の削除
	void RemoveColType(unsigned short colType) { colType_ &= !colType; }
};