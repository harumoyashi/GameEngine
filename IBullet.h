#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "NTimer.h"

class IBullet
{
public:
	enum class BulletType
	{
		LineBullet,
		SideBullet,
		WideBullet,
		Roket,
	};

protected:
	std::unique_ptr<NObj3d> obj_;	//弾のオブジェクト
	NVector2 moveVelo_;				//移動量
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

	bool isAlive_;				//生存フラグ
	float collisionRadius_;		//コライダーの半径
	Sphere collider_;	//弾の当たり判定

	NTimer lifeTimer_;			//生存時間(フレーム数なので注意)

	float damage_;				//与えるダメージ量

	float elapseSpeed_;			//経過時間のスピード(スローモーション用)

public:
	IBullet();
	virtual ~IBullet() = default;
	//生成
	void Generate(const NVector3& pos,const float moveAngle = 0.0f);
	//更新
	void Update();
	//描画
	void Draw();

public:
	// ゲッター //
	//コライダー取得
	inline const Sphere& GetIBulletCollider()const { return collider_; }
	//生存フラグ取得
	inline bool GetisAlive()const { return isAlive_; }
	//移動スピード取得
	inline float GetMoveSpeed()const { return moveSpeed_; }
	//与えるダメージ量取得
	inline float GetDamage()const { return damage_; }
	//座標取得
	inline NVector3& GetPos()const { return obj_->position_; }
	//大きさ取得
	inline NVector3& GetScale()const { return obj_->scale_; }
	//移動用角度取得
	inline float GetMoveAngle()const { return moveAngle_; }

	// セッター //
	//生存フラグ設定
	inline void SetisAlive(const bool isAlive) { isAlive_ = isAlive; }
	//与えるダメージ量設定
	inline void SetDamage(const float damage) { damage_ = damage; }
	//大きさ設定
	inline void SetScale(const float scale) { obj_->scale_ = scale; collisionRadius_ = scale; }
	//移動角度設定
	inline void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//移動スピード設定
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//生存時間設定
	inline void SetLifeTimer(const float lifeTimer) { lifeTimer_ = lifeTimer; }
	//コライダーの半径設定
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
	//経過時間スピード設定
	inline void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};