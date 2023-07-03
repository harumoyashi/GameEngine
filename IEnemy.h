#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class IEnemy
{
private:
	std::unique_ptr<NObj3d> obj_;	//弾のオブジェクト
	NVector2 moveVelo_;				//移動量
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

	bool isAlive_;				//生存フラグ
	uint32_t maxHP_;			//最大体力
	uint32_t hp_;				//現在の体力
	float collisionRadius_;		//コライダーの半径
	SphereCollider collider_;	//弾の当たり判定

	float elapseSpeed_;			//経過時間のスピード(スローモーション用)

public:
	// ゲッター //
	//コライダー取得
	inline const SphereCollider& GetIBulletCollider()const { return collider_; }
	//生存フラグ取得
	inline bool GetisAlive()const { return isAlive_; }
	//移動スピード取得
	inline float GetMoveSpeed()const { return moveSpeed_; }
	//座標取得
	inline NVector3& GetPos()const { return obj_->position_; }
	//大きさ取得
	inline NVector3& GetScale()const { return obj_->scale_; }
	//移動用角度取得
	inline float GetMoveAngle()const { return moveAngle_; }
	
	// セッター //
	//生存フラグ設定
	inline void SetisAlive(const bool isAlive) { isAlive_ = isAlive; }
	//大きさ設定
	inline void SetScale(const float scale) { obj_->scale_ = scale; collisionRadius_ = scale; }
	//移動角度設定
	inline void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//移動スピード設定
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//コライダーの半径設定
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
	//経過時間スピード設定
	inline void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};