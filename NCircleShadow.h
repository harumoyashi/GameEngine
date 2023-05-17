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
	NConstBuff<ConstBuffDataCircleShadow> cbCircleShadow;

private://静的メンバ変数
	NVector3 dir = { 1,0,0 };			// 方向
	float distanceCasterLight = 100.0f;	// キャスターとライトの距離
	NVector3 casterPos = { 0,0,0 };		// キャスター座標(ワールド)
	NVector3 atten = { 0.5f,0.6f,0 };			// 距離減衰係数
	NVector2 factorAngleCos = { 0.2f,0.5f };	// 減衰角度

	//ダーティフラグ
	bool isDirty = false;
	//有効フラグ
	bool isActive = false;

public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	//rootParameterIndex = ルートパラメータの何番目にセットするか
	void Draw(UINT rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	//影の方向をセット
	void SetDir(const NVector3& dir);
	inline const NVector3& GetDir() { return dir; }
	//座標をセット
	void SetCasterPos(const NVector3& casterPos);
	inline const NVector3& GetCasterPos() { return casterPos; }
	//キャスターとライトの距離をセット
	void SetDistanceCasterLight(const float& distanceCasterLight);
	inline const float& GetDistanceCasterLight() { return distanceCasterLight; }
	//減衰係数をセット
	void SetAtten(const NVector3& atten);
	inline const NVector3& GetAtten() { return atten; }
	//減衰角度をセット
	void SetFactorAngle(const NVector2& factorAngle);
	inline const NVector2& GetFactorAngle() { return factorAngleCos; }
	//有効フラグをセット
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//有効フラグを取得
	inline bool GetActive() { return isActive; }
};