#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"
#include "IBullet.h"

class Player final
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//プレイヤーの当たり判定

	bool isAlive_;				//生きてるかフラグ
	bool isDraw_;				//描画するかフラグ

	//------------------------ 移動関連 ------------------------//
	bool isMove_;				//動いてるかフラグ
	NVec2 moveVelo_;			//移動量
	float moveSpeed_;			//移動速度

	float moveAngle_;			//回転角度

	float elapseSpeed_;			//経過時間(こいつが全てに影響を及ぼす)

	//------------------------ ダメージ関連 ------------------------//
	bool isGodmode_;			//無敵状態か
	bool isFlashing_;			//チカチカさせるため用
	NEasing::EaseTimer godmodeTimer_;		//無敵時間

	//------------------------ 弾関連 ------------------------//
	const uint32_t maxBulLevel_ = 5;	//弾の最高レベル(共通)
	uint32_t lineLevel_;				//通常弾のレベル
	uint32_t sideLevel_;				//横方向に出る弾のレベル
	uint32_t wideLevel_;				//左右に角度つけて出る弾のレベル
	uint32_t roketLevel_;				//ロケット弾のレベル

	//------------------------ 死んだとき関連 ------------------------//
	SimpleParticle deadParticle_;				//死んだときに出るパーティクル
	NEasing::EaseTimer deadEffectTimer_;		//死亡演出どのくらいで切り上げるかタイマー
	float slowElapseTime_ = 0.01f;				//死んだときのスローがどのくらいか
	NVec3 deadPos_;								//死んだ座標保存用

	//------------------------ その他 ------------------------//
	NEasing::EaseTimer faildEffectTimer_;		//失敗演出用タイマー

	SimpleParticle clearParticle_;					//クリア時に出るパーティクル
	NEasing::EaseTimer clearParticleTimer_ = 1.0f;	//クリア時に出るパーティクル用タイマー

public:
	Player();
	~Player();
	static Player* GetInstance();

	//初期化
	bool Init();
	//更新
	void Update();
	//クリア時専用の更新
	void ClearUpdate();
	//失敗時専用の更新
	void FaildUpdate();
	//描画
	void Draw();

	//移動処理
	void Move();
	//射撃
	void Shot();

	//何かに当たった時の処理
	void OnCollision();

	//死んだときのパーティクルを出す
	void DeadParticle();

	//指定された弾の種類のレベルを1上げる
	void LevelUp(BulletType bulletType);

	// ゲッター //
	//座標取得
	NVec3 GetPos()const { return obj_->position_; }
	//注視点用にちょい高めの座標取得
	NVec3 GetHeadPos()const { return { obj_->position_.x,obj_->position_.y + obj_->scale_.y,obj_->position_.z }; }
	//前方座標取得
	float GetFrontPosZ()const { return obj_->position_.z + obj_->scale_.z; }
	//大きさ取得
	NVec3 GetScale()const { return obj_->scale_; }
	//無敵状態取得
	bool GetIsGodmode()const { return isGodmode_; }
	//生きてるかフラグ取得
	bool GetIsAlive()const { return isAlive_; }
	//動けるかフラグ取得
	bool GetIsMove()const { return isMove_; }
	//コライダー取得
	const SphereCollider& GetPlayerCollider()const { return collider_; }
	//移動量取得
	NVec2 GetMoveVelo()const { return moveVelo_; }
	//経過時間取得
	float GetElapseSpeed()const { return elapseSpeed_; }
	//オブジェクトの色取得
	NColor& GetColor()const { return obj_->color_; }
	//死亡演出が終わったかどうか取得
	bool GetDeadEffectEnd()const { return deadEffectTimer_.GetEnd(); }

	// セッター //
	//座標設定
	void SetPos(const NVec3& pos) { obj_->position_ = pos; }
	//大きさ設定
	void SetScale(const NVec3& scale) { obj_->scale_ = scale; }
	//アニメーションに経過時間の影響を受けるかフラグ設定
	void SetIsElapseAnime(bool isElapseAnime) { obj_->SetIsElapseAnime(isElapseAnime); }
	//無敵状態設定
	void SetIsGodmode(bool isGodmode) { isGodmode_ = isGodmode; }
	//生きてるかフラグ設定
	void SetIsAlive(bool isAlive);
	//動けるかフラグ設定
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }

private:
	Player& operator=(const Player&) = delete;
};