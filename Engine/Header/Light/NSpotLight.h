#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NVec2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

// スポットライト //

class NSpotLight final
{
public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataSpotLight>> cbSpotLight;

private://静的メンバ変数
	NVec3 lightdir_ = { 0,-1,0 };	// ライト方向
	NVec3 lightpos_ = { 0,10,0 };	// ライト座標(ワールド)
	NVec3 lightcolor_ = NVec3::one;	// ライト色
	NVec3 lightatten_ = NVec3::zero;	// ライト距離減衰係数
	NVec2 lightFactorAngleCos_ = { 0.5f,0.2f };	// ライト減衰角度

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	//コンストラクタ
	NSpotLight();
	//デストラクタ
	~NSpotLight();

	//初期化
	void Init();

	// ゲッター //
	//ライト方向を取得
	const NVec3& GetLightDir()const { return lightdir_; }
	//ライトの座標を取得
	const NVec3& GetLightPos()const { return lightpos_; }
	//ライトの色を取得
	const NVec3& GetLightColor()const { return lightcolor_; }
	//ライトの減衰係数を取得
	const NVec3& GetLightAtten()const { return lightatten_; }
	//ライトの減衰角度を取得
	const NVec2& GetLightFactorAngle()const { return lightFactorAngleCos_; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }

	// セッター //
	//ライト方向をセット
	void SetLightDir(const NVec3& lightdir);
	//ライトの座標をセット
	void SetLightPos(const NVec3& lightpos);
	//ライトの色をセット
	void SetLightColor(const NVec3& lightcolor);
	//ライトの減衰係数をセット
	void SetLightAtten(const NVec3& lightatten);
	//ライトの減衰角度をセット
	void SetLightFactorAngle(const NVec2& lightFactorAngle);
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
};