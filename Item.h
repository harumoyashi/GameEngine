#pragma once
#include "NObj3d.h"
#include "SphereCollider.h"
#include "NEasing.h"

class Item
{
private:
	std::unique_ptr<NObj3d> obj_;	//オブジェクト
	NVec2 moveVelo_;				//移動量
	float moveSpeed_;				//移動スピード

	bool isAlive_;					//生存フラグ
	SphereCollider collider_;		//当たり判定

	float elapseSpeed_;				//経過時間のスピード(スローモーション用)

public:
	Item();
	~Item();

	//生成
	void Generate(const NVec3& pos, const float moveAngle = 0.0f);
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

	// セッター //
	//生存フラグ設定
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};