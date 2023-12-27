#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NVec2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

// 丸影 //

class NCircleShadow final
{
public://サブクラス
	//定数バッファ用データ構造体
	std::unique_ptr<NConstBuff<ConstBuffDataCircleShadow>> cbCircleShadow_;

private://静的メンバ変数
	NVec3 dir_ = { 1,0,0 };			// 方向
	float distanceCasterLight_ = 100.0f;	// キャスターとライトの距離
	NVec3 casterPos_ = NVec3::zero;		// キャスター座標(ワールド)
	NVec3 atten_ = { 0.5f,0.6f,0 };			// 距離減衰係数
	NVec2 factorAngleCos_ = { 0.2f,0.5f };	// 減衰角度

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	NCircleShadow();
	~NCircleShadow();

	//初期化
	void Init();

	//影の方向をセット
	void SetDir(const NVec3& dir);
	const NVec3& GetDir()const { return dir_; }
	//座標をセット
	void SetCasterPos(const NVec3& casterPos);
	const NVec3& GetCasterPos()const { return casterPos_; }
	//キャスターとライトの距離をセット
	void SetDistanceCasterLight(const float distanceCasterLight);
	const float GetDistanceCasterLight()const { return distanceCasterLight_; }
	//減衰係数をセット
	void SetAtten(const NVec3& atten);
	const NVec3& GetAtten()const { return atten_; }
	//減衰角度をセット
	void SetFactorAngle(const NVec2& factorAngle);
	const NVec2& GetFactorAngle()const { return factorAngleCos_; }
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }
};