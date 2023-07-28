#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NCircleShadow final
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
	void Init();

	//影の方向をセット
	void SetDir(const NVector3& dir);
	const NVector3& GetDir()const { return dir_; }
	//座標をセット
	void SetCasterPos(const NVector3& casterPos);
	const NVector3& GetCasterPos()const { return casterPos_; }
	//キャスターとライトの距離をセット
	void SetDistanceCasterLight(const float distanceCasterLight);
	const float GetDistanceCasterLight()const { return distanceCasterLight_; }
	//減衰係数をセット
	void SetAtten(const NVector3& atten);
	const NVector3& GetAtten()const { return atten_; }
	//減衰角度をセット
	void SetFactorAngle(const NVector2& factorAngle);
	const NVector2& GetFactorAngle()const { return factorAngleCos_; }
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }
};