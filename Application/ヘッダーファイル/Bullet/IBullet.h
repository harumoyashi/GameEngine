#pragma once
#include "NObj3d.h"
#include "SphereCollider.h"
#include "NEasing.h"

enum class BulletType
{
	LineBullet,
	SideBullet,
	WideBullet,
	//Roket,

	MaxType
};

class IBullet
{
protected:
	BulletType bulletType;			//弾の種類

	std::unique_ptr<NObj3d> obj_;	//弾のオブジェクト
	NVec2 moveVelo_;				//移動量
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

	bool isAlive_;					//生存フラグ
	SphereCollider collider_;		//弾の当たり判定

	NEasing::EaseTimer lifeTimer_;	//生存時間

	float damage_;					//与えるダメージ量

	float elapseSpeed_;				//経過時間のスピード(スローモーション用)

public:
	IBullet();
	virtual ~IBullet();
	//生成
	void Generate(const NVec3& pos,const float moveAngle = 0.0f);
	//更新
	void Update();
	//描画
	void Draw();

	//何かに当たった時の処理
	void OnCollision();

public:
	// ゲッター //
	//コライダー取得
	const SphereCollider& GetIBulletCollider()const { return collider_; }
	//生存フラグ取得
	bool GetisAlive()const { return isAlive_; }
	//移動スピード取得
	float GetMoveSpeed()const { return moveSpeed_; }
	//与えるダメージ量取得
	float GetDamage()const { return damage_; }
	//座標取得
	NVec3& GetPos()const { return obj_->position_; }
	//大きさ取得
	NVec3& GetScale()const { return obj_->scale_; }
	//移動用角度取得
	float GetMoveAngle()const { return moveAngle_; }

	// セッター //
	//生存フラグ設定
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//与えるダメージ量設定
	void SetDamage(const float damage) { damage_ = damage; }
	//大きさ設定
	void SetScale(const float scale) { obj_->scale_ = scale; collider_.SetRadius(scale); }
	//移動角度設定
	void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//移動スピード設定
	void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//生存時間設定
	void SetLifeTimer(const float lifeTimer) { lifeTimer_ = lifeTimer; }
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};