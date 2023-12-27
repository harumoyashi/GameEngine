#pragma once
#include "NObj3d.h"
#include "CircleCollider.h"
#include "NEasing.h"
#include "IBullet.h"

// アイテム //

class Item
{
private:
	std::unique_ptr<NObj3d> obj_;	//オブジェクト
	NVec2 moveVelo_;				//移動量
	float moveSpeed_;				//移動スピード

	bool isAlive_;					//生存フラグ
	CircleCollider collider_;		//当たり判定

	float elapseSpeed_;				//経過時間のスピード(スローモーション用)

	BulletType bulletType_;			//弾の種類

public:
	Item();
	~Item();

	//生成
	//isPop:生成時に跳ねるか
	void Generate(const NVec3& pos,bool isPop = true);
	//更新
	void Update();
	//描画
	void Draw();

	//何かに当たった時の処理
	void OnCollision();

public:
	// ゲッター //
	//コライダー取得
	const CircleCollider& GetIBulletCollider()const { return collider_; }
	//生存フラグ取得
	bool GetisAlive()const { return isAlive_; }
	//座標取得
	NVec3 GetPos()const { return obj_->position_; }
	//弾の種類
	BulletType GetBulletType()const { return bulletType_; }

	// セッター //
	//生存フラグ設定
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
	//弾の種類設定
	void SetBulletType(BulletType bulletType) { bulletType_ = bulletType; }
};