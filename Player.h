#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"

class Player final
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//プレイヤーの当たり判定

	bool isAlive_;				//生きてるかフラグ
	bool isDraw_;				//描画するかフラグ

	//------------------------ 移動関連 ------------------------//
	bool isMove_;				//動いてるかフラグ
	NVector2 moveVelo_;			//移動量
	float moveSpeed_;			//移動速度

	float moveAngle_;			//回転角度

	float elapseSpeed_;			//経過時間(こいつが全てに影響を及ぼす)

	//------------------------ ダメージ関連 ------------------------//
	bool isGodmode_;			//無敵状態か
	bool isFlashing_;			//チカチカさせるため用
	NEasing::EaseTimer godmodeTimer_;		//無敵時間

	//------------------------ 弾関連 ------------------------//
	uint32_t lineLevel_;		//通常弾のレベル
	uint32_t sideLevel_;		//横方向に出る弾のレベル
	uint32_t wideLevel_;		//左右に角度つけて出る弾のレベル
	uint32_t roketLevel_;		//ロケット弾のレベル

	//------------------------ その他 ------------------------//
	SimpleParticle deadParticle_;				//死んだときに出るパーティクル
	NEasing::EaseTimer deadEffectTimer_;		//死亡演出どのくらいで切り上げるかタイマー
	float slowElapseTime_ = 0.01f;				//死んだときのスローがどのくらいか

public:
	Player();
	~Player();
	static Player* GetInstance();

	//初期化
	bool Init();
	//更新
	void Update();
	//描画
	void Draw();
	void DrawSprite();

	//移動処理
	void Move();
	//射撃
	void Shot();

	//何かに当たった時の処理
	void OnCollision();

	//死んだときのパーティクルを出す
	void DeadParticle();

	// ゲッター //
	//座標取得
	NVector3 GetPos()const { return obj_->position_; }
	//前方座標取得
	float GetFrontPosZ()const { return obj_->position_.z + obj_->scale_.z; }
	//大きさ取得
	NVector3 GetScale()const { return obj_->scale_; }
	//無敵状態取得
	bool GetIsGodmode()const { return isGodmode_; }
	//生きてるかフラグ取得
	bool GetIsAlive()const { return isAlive_; }
	//動けるかフラグ取得
	bool GetIsMove()const { return isMove_; }
	//コライダー取得
	const SphereCollider& GetPlayerCollider()const { return collider_; }
	//移動量取得
	NVector2 GetMoveVelo()const { return moveVelo_; }
	//経過時間取得
	float GetElapseSpeed()const { return elapseSpeed_; }
	//オブジェクトの色取得
	NColor& GetColor()const { return obj_->color_; }
	//死亡演出が終わったかどうか取得
	bool GetDeadEffectEnd()const { return deadEffectTimer_.GetEnd(); }

	// セッター //
	//座標設定
	void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//大きさ設定
	void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//無敵状態設定
	void SetIsGodmode(bool isGodmode) { isGodmode_ = isGodmode; }
	//生きてるかフラグ設定
	void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	//動けるかフラグ設定
	void SetIsMove(bool isMove) { isMove_ = isMove; }
};