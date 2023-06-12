#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NCircleShadow
{
public://サブクラス
	//定数バッファ用データ構造体
	std::unique_ptr<NConstBuff<ConstBuffDataCircleShadow>> cbCircleShadow_;

private://静的メンバ変数
	NVector3 dir_ = { 1,0,0 };			// 方向
	float distanceCasterLight_ = 100.0f;	// キャスターとライトの距離
	NVector3 casterPos_ = { 0,0,0 };		// キャスター座標(ワールド)
	NVector3 atten_ = { 0.5f,0.6f,0 };			// 距離減衰係数
	NVector2 factorAngleCos_ = { 0.2f,0.5f };	// 減衰角度

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	NCircleShadow();
	~NCircleShadow();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	//rootParameterIndex = ルートパラメータの何番目にセットするか
	void Draw(const uint32_t rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	//影の方向をセット
	void SetDir(const NVector3& dir);
	inline const NVector3& GetDir() { return dir_; }
	//座標をセット
	void SetCasterPos(const NVector3& casterPos);
	inline const NVector3& GetCasterPos() { return casterPos_; }
	//キャスターとライトの距離をセット
	void SetDistanceCasterLight(const float distanceCasterLight);
	inline const float& GetDistanceCasterLight() { return distanceCasterLight_; }
	//減衰係数をセット
	void SetAtten(const NVector3& atten);
	inline const NVector3& GetAtten() { return atten_; }
	//減衰角度をセット
	void SetFactorAngle(const NVector2& factorAngle);
	inline const NVector2& GetFactorAngle() { return factorAngleCos_; }
	//有効フラグをセット
	inline void SetActive(const bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	inline bool GetActive() { return isActive_; }
};