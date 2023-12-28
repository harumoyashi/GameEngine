#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "CircleCollider.h"
#include "SimpleParticle.h"

// ただ追いかけてくるボス //

class Boss
{
private:
	std::unique_ptr<NObj3d> obj_;	//オブジェクト
	CircleCollider collider_;		//コライダー
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

	//登場演出
	NEasing::EaseTimer entryTimer_ = 2.5f;		//登場演出タイマー
	NEasing::EaseTimer scalingTimer_ = 0.4f;	//おっきくなるタイマー
	SimpleParticle landingParticle_;			//着地時に出るパーティクル

public:
	//コンストラクタ
	Boss();
	//デストラクタ
	~Boss();

	//シングルトンインスタンス取得
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
	//座標取得
	NVec3 GetPos() { return obj_->position_; }
	//頭らへんの座標取得
	NVec3 GetHeadPos() { return obj_->position_ + NVec3(0, obj_->scale_.y, 0); }

	//登場演出中か
	bool GetIsEntry() { return entryTimer_.GetRun(); }
	//おっきくなるの終わったか
	bool GetIsScalingEnd() { return scalingTimer_.GetEnd(); }

	// セッター //
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};