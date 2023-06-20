#pragma once
#include <NVector2.h>
#include <NColor.h>
#include <NTexture.h>
#include <NEasing.h>

class Emitter2D
{
	//ポータルに吸い込まれてく粒子1粒
	struct Particle2D
	{
		//座標
		NVector2 pos;
		//大きさ
		NVector2 scale;
		NVector2 startScale;	//開始時の大きさ
		NVector2 endScale;		//終了時の大きさ
		//角度
		float rot = 0;
		//速度
		NVector2 velo;
		//加速度
		NVector2 accel;
		//重力
		float gravity = 0.98f;

		//色
		NColor color;
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;

		//イージング用タイマー
		NEasing::easeTimer timer = 1.0f;

		//距離
		float radius = 0;
		float startRadius = 0;	//開始時の距離

		//画像
		TextureHandle key;
	};

private:
	//座標
	NVector2 pos_;
	//角度
	float rot_ = 0.0f;
	//大きさ
	NVector2 scale_{ 1.f,1.f };
	float minScale_;	//エミッターの最小サイズ(比率)
	float maxScale_;	//エミッターの最大サイズ(比率)
	//元の大きさ
	NVector2 originalScale_;
	//拡縮用倍率
	float scaling_;
	//拡縮用タイマー
	NEasing::easeTimer scalingTimer_ = 1.0f;
	//パーティクル出すタイミング用タイマー
	int particleTimer_ = 0;
	//何フレームに一回パーティクル追加するか
	int addInterval_;

	//画像
	TextureHandle key_;

	static const int maxCount_ = 256;	//最大数
	std::vector<Particle2D> particles_;	//パーティクル配列

	bool isActive_ = false;

public:
	Emitter2D();
	//初期化
	void Init();
	//更新
	void Update(bool isGravity = false);
	//描画
	void DrawBox();
	void DrawGraph();

	//パーティクル追加
	void Add(int addNum, int life, float minScale, float maxScale, NVector2 minVelo, NVector2 maxVelo,
		NVector2 accel = { 0,0 }, float minRot = 0.0f, float maxRot = 0.0f, NColor color = NColor::kWhite);
	//パーティクル全消し
	inline void ClearParticles() { particles_.clear(); }

	//座標取得
	inline NVector2 GetPos() { return pos_; }
	//座標設定
	inline void SetPos(float x, float y) { pos_ = { x,y }; }
	inline void SetPos(NVector2 pos) { pos_ = pos; }
	//大きさ取得
	inline NVector2 GetScale() { return scale_; }
	//大きさ設定
	void SetScale(NVector2 scale);
	//角度設定
	inline void SetRot(float rot) { rot_ = rot; }

	//有効フラグ設定
	inline void SetIsActive(bool isActive) { isActive_ = isActive; }

	//パーティクル全部死んだか
	inline bool GetParticlesDead() { return particles_.empty(); }

	//何個パーティクルあるか
	inline size_t GetParticlesSize() { return particles_.size(); }

	//テクスチャの設定
	void SetTextureKey(TextureHandle key) { key_ = key; }

	//拡縮用タイマーが切り替わる時間設定(秒)
	//isRun:true = プラスされてく,false:マイナスされてく
	void SetScalingTimer(float timer);
	void StartScalingTimer(bool isRun = true);
};