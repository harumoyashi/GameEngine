#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"

class IEnemy
{
public:
	//ここのタイプに応じてファクトリーで配置して生成する
	enum class EnemyType
	{
		MouseSide,	//横からくるねずみたち
		MouseFront,	//正面からくるねずみたち
	};

protected:
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

	std::string enemyTypeName_;		//敵の種族名
	uint32_t enemyNum_;				//敵の識別番号
	SimpleParticle deadParticle_;	//死んだときに出すパーティクル

	bool isItem_;					//アイテム持ってるかフラグ
	bool isDraw_;					//描画するかフラグ

public:
	IEnemy();
	virtual ~IEnemy();

	//生成
	//pos:配置する座標
	//moveAngle:移動する角度
	//modelname:モデルの名前(ID)
	void Generate(const NVec3& pos, const float moveAngle, const std::string& modelname);

	//初期化
	virtual void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//何かに当たった時の処理
	void OnCollision();

	//エミッターの登録
	//enemyNum:敵の識別番号
	void AddEmitter(uint32_t eneNum);
	//死んだときのパーティクルを出す
	void DeadParticle();
	//画面内にいるか判定
	bool IsInScreen();
	//生きてていい範囲内にいるか判定していなければ殺す
	void IsInActiveArea();

	//---------------------------- 継承するやつら ----------------------------//
	//移動
	virtual void Move();

	// ゲッター //
	//コライダー取得
	const SphereCollider GetCollider()const { return collider_; }
	//生存フラグ取得
	bool GetisAlive()const { return isAlive_; }
	//移動スピード取得
	float GetMoveSpeed()const { return moveSpeed_; }
	//座標取得
	NVec3& GetPos()const { return obj_->position_; }
	//大きさ取得
	NVec3& GetScale()const { return obj_->scale_; }
	//移動用角度取得
	float GetMoveAngle()const { return moveAngle_; }
	//パーティクルが1つも出てないかフラグ取得
	bool GetParticlesDead()const { return deadParticle_.GetParticlesDead(); }

	// セッター //
	//生存フラグ設定
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//座標設定
	void SetPos(const NVec3 pos) { obj_->position_ = pos; collider_.SetCenterPos(pos); }
	//大きさ設定
	void SetScale(const float scale) {
		obj_->scale_ = scale;  collider_.SetRadius(scale);
	}
	//スケールいじる用変数設定
	void SetAddScale(const float scale) { addScale_ = scale; }
	//移動角度設定
	void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//移動スピード設定
	void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
	//敵の識別番号設定
	void SetEnemyNum(const uint32_t eneNum) { enemyNum_ = eneNum; }
	//アイテム持ってるかフラグ設定
	void SetisItem(bool isItem);
};