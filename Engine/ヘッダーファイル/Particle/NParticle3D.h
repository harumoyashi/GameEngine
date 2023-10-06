#pragma once
#include "NVec3.h"
#include "NColor.h"
#include "NTexture.h"
#include "NLightGroup.h"
#include "NConstBuff.h"
#include "NVertexBuff.h"
#include "NEasing.h"
#include "NTimer.h"
#include "NUtil.h"

class IEmitter3D
{
	//粒子1粒
	struct Particle3D
	{
		//座標
		NVec3 pos;
		//大きさ
		float scale;
		float startScale;	//開始時の大きさ
		float endScale;		//終了時の大きさ
		//角度
		NVec3 rot;
		NVec3 plusRot;	//更新処理で回転させるときに使う用
		//速度
		NVec3 velo;
		//加速度
		NVec3 accel;
		//重力
		float gravity = 0.98f;

		//色
		NColor color;
		//生存時間
		NEasing::EaseTimer aliveTimer;

		//イージング用タイマー
		NEasing::EaseTimer easeTimer = 1.0f;

		//エミッター座標からの距離
		float radius = 0;
		float startRadius = 0;	//開始時の距離

		//画像
		//TextureHandle key;
	};

private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;	//3D変換行列

	std::vector<NVertexParticle>vertices_;		//頂点群
	NVertexBuff<NVertexParticle> vertexBuff_;	//頂点バッファ

	NMatrix4 matWorld_;	//3D変換行列

	// ライト
	static NLightGroup* sLightGroup;

	//座標
	NVec3 pos_{};
	//角度
	NVec3 rot_{};
	//大きさ
	NVec3 scale_{ 0.1f,0.1f,0.1f };	//これ0にするといかれる(原因不明)
	float minScale_;	//パーティクルの最小サイズ
	float maxScale_;	//パーティクルの最大サイズ
	//元の大きさ
	NVec3 originalScale_;
	//拡縮用倍率
	float scaling_;
	//拡縮用タイマー
	NEasing::EaseTimer scalingTimer_ = 1.0f;
	//パーティクル出すタイミング用タイマー
	uint32_t particleTimer_ = 0;
	//何フレームに一回パーティクル追加するか
	uint32_t addInterval_;

	//経過時間のスピード(スローモーション用)
	float elapseSpeed_;

	const uint32_t maxParticle_ = 256;		//最大数
	std::vector<Particle3D> particles_;		//パーティクル配列

	bool isActive_ = true;					//有効にするかフラグ

	bool isGravity_ = false;				//重力の影響受けるかフラグ
	bool isRotation_ = false;				//回すかフラグ

	NTexture texture_;						//テクスチャ(使うかわからん)

public:
	IEmitter3D();
	virtual ~IEmitter3D() = default;

	//初期化
	//このままの処理を呼びたいなら継承先でこれを呼ぶ
	virtual void Init();
	//更新
	//このままの処理を呼びたいなら継承先でこれを呼ぶ
	virtual void Update();
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//ブレンドモード設定
	static void SetBlendMode(BlendMode blendMode);
	//描画
	void Draw();

	//ワールド行列の合成
	void UpdateMatrix();
	//定数バッファへ送信
	void TransferMatrix();

	//パーティクル追加(固有処理にしたかったらoverrideで上書きする)
	//life:秒数指定なので注意
	virtual void Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
		NVec3 minVelo, NVec3 maxVelo, NVec3 accel = {}, NVec3 minRot = {}, NVec3 maxRot = {}) = 0;
	//パーティクル全消し
	void ClearParticles() { particles_.clear(); }

	//ゲッター//
	//座標取得
	NVec3 GetPos()const { return pos_; }
	//大きさ取得
	NVec3 GetScale()const { return scale_; }
	//パーティクル全部死んだか取得
	bool GetParticlesDead()const { return particles_.empty(); }
	//何個パーティクルあるか取得
	size_t GetParticlesSize()const { return particles_.size(); }
	//有効フラグ取得
	bool GetIsActive()const { return isActive_; }

	//セッター//
	//座標設定
	void SetPos(float x, float y, float z) { pos_ = { x,y,z }; }
	void SetPos(const NVec3& pos) { pos_ = pos; }
	//大きさ設定
	void SetScale(const NVec3& scale);
	//角度設定
	void SetRot(float rot) { rot_ = rot; }
	//ライトを設定
	static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
	//有効フラグ設定
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	//重力フラグ設定
	void SetIsGravity(bool isGravity) { isGravity_ = isGravity; }
	//回転フラグ設定
	void SetIsRotation(bool isRotation) { isRotation_ = isRotation; }

	//拡縮用タイマーが切り替わる時間設定(秒)
	void SetScalingTimer(float timer);
	//拡縮用タイマー開始
	//isRun:true = プラスされてく,false:マイナスされてく
	void StartScalingTimer(bool isRun = true);

	//経過時間スピード設定
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};