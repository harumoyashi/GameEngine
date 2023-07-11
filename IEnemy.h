#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class IEnemy
{
public:
	//ここのタイプに応じてファクトリーで配置して生成する
	enum class EnemyType
	{
		WolfSide,	//横からくる狼たち
	};

protected:
	std::unique_ptr<NObj3d> obj_;	//オブジェクト
	NVector2 moveVelo_;				//移動量
	float moveAngle_;				//移動用角度
	float moveSpeed_;				//移動スピード

	bool isAlive_;					//生存フラグ
	uint32_t maxHP_;				//最大体力
	uint32_t hp_;					//現在の体力
	float collisionRadius_;			//コライダーの半径
	Sphere collider_;		//当たり判定

	float elapseSpeed_;				//経過時間のスピード(スローモーション用)

	//アイテム持たせる

	//コイン(ゲージ)持たせる


public:
	IEnemy();
	virtual ~IEnemy() = default;

	//初期化
	//pos:配置する座標
	//moveAngle:移動する角度
	//modelname:モデルの名前(ID)
	void Generate(const NVector3& pos, const float moveAngle, const std::string& modelname);
	//更新
	void Update();
	//描画
	void Draw();

	//---------------------------- 継承するやつら ----------------------------//
	//移動
	virtual void Move();

	// ゲッター //
	//コライダー取得
	inline const Sphere& GetCollider()const { return collider_; }
	//生存フラグ取得
	inline bool GetisAlive()const { return isAlive_; }
	//移動スピード取得
	inline float GetMoveSpeed()const { return moveSpeed_; }
	//座標取得
	inline NVector3& GetPos()const { return obj_->position_; }
	//大きさ取得
	inline NVector3& GetScale()const { return obj_->scale_; }
	//移動用角度取得
	inline float GetMoveAngle()const { return moveAngle_; }

	// セッター //
	//生存フラグ設定
	inline void SetisAlive(const bool isAlive) { isAlive_ = isAlive; }
	//座標設定
	inline void SetPos(const NVector3 pos) { obj_->position_ = pos; collider_.centerPos = pos; }
	//大きさ設定
	inline void SetScale(const float scale) {
		obj_->scale_ = scale; collisionRadius_ = scale; collider_.radius = scale; }
	//移動角度設定
	inline void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//移動スピード設定
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//コライダーの半径設定
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
	//経過時間スピード設定
	inline void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};