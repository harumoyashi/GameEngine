#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NSpotLight
{
public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataSpotLight>> cbSpotLight;

private://静的メンバ変数
	NVector3 lightdir_ = { 0,-1,0 };	// ライト方向
	NVector3 lightpos_ = { 0,10,0 };	// ライト座標(ワールド)
	NVector3 lightcolor_ = { 1,1,1 };	// ライト色
	NVector3 lightatten_ = { 0,0,0 };	// ライト距離減衰係数
	NVector2 lightFactorAngleCos_ = { 0.5f,0.2f };	// ライト減衰角度

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	NSpotLight();
	~NSpotLight();

	//初期化
	void Init();

	void SetLightDir(const NVector3& lightdir);
	inline const NVector3& GetLightDir()const { return lightdir_; }
	//ライトの座標をセット
	void SetLightPos(const NVector3& lightpos);
	inline const NVector3& GetLightPos()const { return lightpos_; }
	//ライトの色をセット
	void SetLightColor(const NVector3& lightcolor);
	inline const NVector3& GetLightColor()const { return lightcolor_; }
	//ライトの減衰係数をセット
	void SetLightAtten(const NVector3& lightatten);
	inline const NVector3& GetLightAtten()const { return lightatten_; }
	//ライトの減衰角度をセット
	void SetLightFactorAngle(const NVector2& lightFactorAngle);
	inline const NVector2& GetLightFactorAngle()const { return lightFactorAngleCos_; }
	//有効フラグをセット
	inline void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	inline bool GetActive()const { return isActive_; }
};