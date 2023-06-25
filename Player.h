#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

#include "Bullet.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//プレイヤーの当たり判定

	bool isAlive_;		//生きてるかフラグ
	bool isCanMove_;	//動けるかフラグ
	bool isDraw_;		//描画するかフラグ

	float collisionRadius_;	//コライダーの半径

	// ダメージ関連
	bool isGodmode_;		//無敵状態か
	bool isFlashing_;		//チカチカさせるため用
	NTimer godmodeTimer_;	//無敵時間

	//弾関連//
	std::list<std::unique_ptr<Bullet>> bullets_;	//弾ども
	//NTimer shotTimer_;		//弾撃つ時間
	NTimer shotCoolTimer_;		//弾撃つまでの時間
	bool isCanShot_;			//撃てるかフラグ

public:
	Player();
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	// ゲッター //
	//座標取得
	inline NVector3 GetPos() { return obj_->position_; }
	//大きさ取得
	inline NVector3 GetScale() { return obj_->scale_; }
	//コライダーの半径取得
	inline float GetCollisionRadius() { return collisionRadius_; }
	//弾ども取得
	inline std::list<std::unique_ptr<Bullet>>* GetBullets() { return &bullets_; }
	//無敵状態取得
	inline bool GetisGodmode() { return isGodmode_; }
	//生きてるかフラグ取得
	inline bool GetisAlive() { return isAlive_; }
	//動けるかフラグ取得
	inline bool GetisCanMove() { return isCanMove_; }
	//コライダー取得
	inline const SphereCollider& GetPlayerCollider()const { return collider_; }

	// セッター //
	//座標設定
	inline void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//大きさ設定
	inline void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//無敵状態設定
	inline void SetisGodmode(const bool& isGodmode) { isGodmode_ = isGodmode; }
	//コライダーの半径設定
	inline void SetCollisionRadius(const float& collisionRadius) { collisionRadius_ = collisionRadius; }
	//動けるかフラグ設定
	inline void SetisCanMove(const bool& isCanMove) { isCanMove_ = isCanMove; }
};