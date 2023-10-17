#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NConstBuff.h"

class NPointLight final
{
public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataPointLight>> cbPointLight;

private://静的メンバ変数
	NVec3 lightpos_ = NVec3::zero;		// ライト座標(ワールド)
	NVec3 lightcolor_ = NVec3::one;	// ライト色
	NVec3 lightatten_ = { 0.2f,0.2f,0.2f };	// ライト距離減衰係数

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	NPointLight();
	~NPointLight();

	//初期化
	void Init();

	//ライトの座標をセット
	void SetLightPos(const NVec3& lightpos);
	const NVec3& GetLightPos()const { return lightpos_; }
	//ライトの色をセット
	void SetLightColor(const NVec3& lightcolor);
	const NVec3& GetLightColor()const { return lightcolor_; }
	//ライトの減衰係数をセット
	void SetLightAtten(const NVec3& lightatten);
	const NVec3& GetLightAtten()const { return lightatten_; }
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }
};