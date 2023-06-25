#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class Bullet
{
private:
	std::unique_ptr<NObj3d> obj;	//弾のオブジェクト
	float moveAngle;				//移動用角度
	float moveSpeed;				//移動スピード

private:
	bool isActive;				//有効フラグ
	float collisionRadius;		//コライダーの半径
	SphereCollider collider;	//弾の当たり判定

	float damage;				//与えるダメージ量

	bool isCanGenerate;			//弾生成できるフラグ

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
	inline const SphereCollider& GetBulletCollider()const { return collider; }
	//有効フラグ取得
	inline bool GetisActive()const { return isActive; }
	//弾生成できるフラグ取得
	inline bool GetisCanGenerate()const { return isCanGenerate; }
	//移動スピード取得
	inline float GetMoveSpeed()const { return moveSpeed; }
	//与えるダメージ量取得
	inline float GetDamage()const { return damage; }
	//座標取得
	inline NVector3& GetPos()const { return obj->position_; }
	//大きさ取得
	inline NVector3& GetScale()const { return obj->scale_; }
	//移動用角度取得
	inline float GetMoveAngle()const { return moveAngle; }

public:
	// セッター //
	//有効フラグ設定
	inline void SetisActive(const bool& isActive) { this->isActive = isActive; }
	//弾生成できるフラグ設定
	inline void SetisCanGenerate(const bool& isCanGenerate) { this->isCanGenerate = isCanGenerate; }
	//与えるダメージ量設定
	inline void SetDamage(const float& damage) { this->damage = damage; }
	//大きさ設定
	inline void SetScale(const float& scale) { obj->scale_ = scale; collisionRadius = scale; }
	//移動スピード設定
	inline void SetMoveSpeed(const float& moveSpeed) { this->moveSpeed = moveSpeed; }
	//色設定
	inline void SetColor(const NColor& color) { obj->color_ = color; }
	//コライダーの半径設定
	inline void SetColRadius(const float& radius) { collisionRadius = collisionRadius * radius; }
};