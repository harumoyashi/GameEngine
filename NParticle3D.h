#pragma once
#include <NVector3.h>
#include <NColor.h>
#include <NTexture.h>
#include <NConstBuff.h>
#include <NVertexBuff.h>
#include <NEasing.h>

class IEmitter3D
{
	//粒子1粒
	struct Particle3D
	{
		//座標
		NVector3 pos;
		//大きさ
		NVector3 scale;
		NVector3 startScale;	//開始時の大きさ
		NVector3 endScale;		//終了時の大きさ
		//角度
		float rot = 0;
		//速度
		NVector3 velo;
		//加速度
		NVector3 accel;
		//重力
		float gravity = 0.98f;

		//色
		NColor color;
		//現在フレーム
		uint32_t frame = 0;
		//終了フレーム
		uint32_t num_frame = 0;

		//イージング用タイマー
		NEasing::EaseTimer timer = 1.0f;

		//距離
		float radius = 0;
		float startRadius = 0;	//開始時の距離

		//画像
		TextureHandle key;
	};

private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;	//3D変換行列

	std::vector<NVertexParticle>vertices_;		//頂点群
	NVertexBuff<NVertexParticle> vertexBuff_;	//頂点バッファ

	NMatrix4 matWorld_;	//3D変換行列

	//座標
	NVector3 pos_;
	//角度
	float rot_ = 0.0f;
	//大きさ
	NVector3 scale_{ 1.f,1.f,1.f };
	float minScale_;	//エミッターの最小サイズ(比率)
	float maxScale_;	//エミッターの最大サイズ(比率)
	//元の大きさ
	NVector3 originalScale_;
	//拡縮用倍率
	float scaling_;
	//拡縮用タイマー
	NEasing::EaseTimer scalingTimer_ = 1.0f;
	//パーティクル出すタイミング用タイマー
	uint32_t particleTimer_ = 0;
	//何フレームに一回パーティクル追加するか
	uint32_t addInterval_;

	const uint32_t maxParticle_ = 256;	//最大数
	std::vector<Particle3D> particles_;		//パーティクル配列

	bool isActive_ = false;					//有効にするかフラグ

	NTexture texture_;						//テクスチャ(使うかわからん)

public:
	IEmitter3D();
	virtual ~IEmitter3D() = default;

	//初期化
	void Init();
	//更新
	void Update(bool isGravity = false);
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//描画
	void Draw();

	//パーティクル追加(固有処理にしたかったらoverrideで上書きする)
	virtual void Add(uint32_t addNum, uint32_t life, float minScale, float maxScale, NVector3 minVelo, NVector3 maxVelo,
		NVector3 accel = { 0,0,0 }, float minRot = 0.0f, float maxRot = 0.0f, NColor color = NColor::kWhite);
	//パーティクル全消し
	inline void ClearParticles() { particles_.clear(); }

	//ゲッター//
	//座標取得
	inline NVector3 GetPos()const { return pos_; }
	//大きさ取得
	inline NVector3 GetScale()const { return scale_; }

	//パーティクル全部死んだか取得
	inline bool GetParticlesDead()const { return particles_.empty(); }

	//何個パーティクルあるか取得
	inline size_t GetParticlesSize()const { return particles_.size(); }

	//セッター//
	//座標設定
	inline void SetPos(float x, float y, float z) { pos_ = { x,y,z }; }
	inline void SetPos(NVector3& pos) { pos_ = pos; }
	//大きさ設定
	void SetScale(NVector3& scale);
	//角度設定
	inline void SetRot(float rot) { rot_ = rot; }

	//有効フラグ設定
	inline void SetIsActive(bool isActive) { isActive_ = isActive; }

	//拡縮用タイマーが切り替わる時間設定(秒)
	void SetScalingTimer(float timer);
	//拡縮用タイマー開始
	//isRun:true = プラスされてく,false:マイナスされてく
	void StartScalingTimer(bool isRun = true);
};