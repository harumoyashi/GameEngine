#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//プレイヤーの当たり判定

	bool isAlive_;		//生きてるかフラグ
	bool isDraw_;		//描画するかフラグ

	float collisionRadius_;	//コライダーの半径

	//------------------------ 移動関連 ------------------------//
	bool isMove_;				//動いてるかフラグ
	NVector2 moveVelo_;			//移動量
	float moveSpeed_;			//移動速度

	float moveAngle_;				//回転角度

	//------------------------ ダメージ関連 ------------------------//
	bool isGodmode_;		//無敵状態か
	bool isFlashing_;		//チカチカさせるため用
	NTimer godmodeTimer_;	//無敵時間

	//------------------------ 弾関連 ------------------------//
	uint32_t lineLevel_;		//通常弾のレベル
	uint32_t sideLevel_;		//横方向に出る弾のレベル
	uint32_t wideLevel_;		//左右に角度つけて出る弾のレベル
	uint32_t roketLevel_;		//ロケット弾のレベル

public:
	Player();
	static Player* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//移動処理
	void Move();
	//射撃
	void Shot();

	// ゲッター //
	//座標取得
	inline NVector3 GetPos()const { return obj_->position_; }
	//前方座標取得
	inline float GetFrontPosZ()const { return obj_->position_.z + obj_->scale_.z; }
	//大きさ取得
	inline NVector3 GetScale()const { return obj_->scale_; }
	//コライダーの半径取得
	inline float GetCollisionRadius()const { return collisionRadius_; }
	//無敵状態取得
	inline bool GetIsGodmode()const { return isGodmode_; }
	//生きてるかフラグ取得
	inline bool GetIsAlive()const { return isAlive_; }
	//動けるかフラグ取得
	inline bool GetIsMove()const { return isMove_; }
	//コライダー取得
	inline const SphereCollider& GetPlayerCollider()const { return collider_; }
	//移動量取得
	inline NVector2 GetMoveVelo()const { return moveVelo_; }

	// セッター //
	//座標設定
	inline void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//大きさ設定
	inline void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//無敵状態設定
	inline void SetIsGodmode(const bool isGodmode) { isGodmode_ = isGodmode; }
	//生きてるかフラグ設定
	inline void SetIsAlive(const bool isAlive) { isAlive_ = isAlive; }
	//コライダーの半径設定
	inline void SetCollisionRadius(const float collisionRadius) { collisionRadius_ = collisionRadius; }
	//動けるかフラグ設定
	inline void SetIsMove(const bool isMove) { isMove_ = isMove; }
};