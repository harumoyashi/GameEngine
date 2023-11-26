#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"

class Boss
{
private:
	std::unique_ptr<NObj3d> obj_;	//オブジェクト
	SphereCollider collider_;		//コライダー
	NVec3 oriScale_;				//元のスケール保存用(リズム乗らすときずらすから)
	NVec3 addScale_;				//スケールいじる用(リズム乗らすときずらすから)
	NVec2 moveVelo_;				//移動量
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

	bool isAlive_;					//生存フラグ
	uint32_t maxHP_;				//最大体力
	uint32_t hp_;					//現在の体力
	uint32_t score_;				//倒すことで獲得できる得点(本家の10倍にする)

	float elapseSpeed_;				//経過時間のスピード(スローモーション用)
	float homingPower_;				//ホーミングのパワー

	SimpleParticle deadParticle_;	//死んだときに出すパーティクル

public:
	Boss();
	~Boss();

	static Boss* GetInstance();

	//生成
	//pos:配置する座標
	void Generate(const NVec3& pos);

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//何かに当たった時の処理
	void OnCollision();

	//死んだときのパーティクルを出す
	void DeadParticle();

	//移動
	void Move();

	// ゲッター //

	// セッター //
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};