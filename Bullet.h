#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class Bullet
{
private:
	std::unique_ptr<NObj3d> obj_;	//弾のオブジェクト
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

private:
	bool isActive_;				//有効フラグ
	float collisionRadius_;		//コライダーの半径
	SphereCollider collider_;	//弾の当たり判定

	float damage_;				//与えるダメージ量

	bool isCanGenerate_;		//弾生成できるフラグ

public:
	Bullet();
	//生成
	void Generate(const NVector3& pos);
	//更新
	void Update();
	//描画
	void Draw();
public:
	// ゲッター //
	//コライダー取得
	inline const SphereCollider& GetBulletCollider()const { return collider_; }
	//有効フラグ取得
	inline bool GetisActive()const { return isActive_; }
	//弾生成できるフラグ取得
	inline bool GetisCanGenerate_()const { return isCanGenerate_; }
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

public:
	// セッター //
	//有効フラグ設定
	inline void SetisActive(const bool isActive) { isActive_ = isActive; }
	//弾生成できるフラグ設定
	inline void SetisCanGenerate_(const bool isCanGenerate) { isCanGenerate_ = isCanGenerate; }
	//与えるダメージ量設定
	inline void SetDamage(const float damage) { damage_ = damage; }
	//大きさ設定
	inline void SetScale(const float scale) { obj_->scale_ = scale; collisionRadius_ = scale; }
	//移動スピード設定
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//色設定
	inline void SetColor(const NColor& color) { obj_->color_ = color; }
	//コライダーの半径設定
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
};