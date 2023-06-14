#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NPointLight
{
public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataPointLight>> cbPointLight;

private://静的メンバ変数
	NVector3 lightpos_ = { 0,0,0 };		// ライト座標(ワールド)
	NVector3 lightcolor_ = { 1,1,1 };	// ライト色
	NVector3 lightatten_ = { 0.2f,0.2f,0.2f };	// ライト距離減衰係数

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public://メンバ関数
	NPointLight();
	~NPointLight();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	//rootParameterIndex = ルートパラメータの何番目にセットするか
	void Draw(const uint32_t rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	//ライトの座標をセット
	void SetLightPos(const NVector3& lightpos);
	inline NVector3 GetLightPos() { return lightpos_; }
	//ライトの色をセット
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor_; }
	//ライトの減衰係数をセット
	void SetLightAtten(const NVector3& lightatten);
	inline NVector3 GetLightAtten() { return lightatten_; }
	//有効フラグをセット
	inline void SetActive(const bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	inline bool GetActive() { return isActive_; }
};