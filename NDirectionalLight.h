#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NDirectionalLight
{
private://静的メンバ変数
	NVector3 lightdir = { 1,0,0 };	 // ライト光線方向
	NVector3 lightcolor = { 1,1,1 }; // ライト色

	//ダーティフラグ
	bool isDirty = false;
	//有効フラグ
	bool isActive = false;

public:
	//定数バッファ
	NConstBuff<ConstBuffDataLight> cbLight;

public: //メンバ関数
	NDirectionalLight();

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	//rootParameterIndex = ルートパラメータの何番目にセットするか
	void Draw(UINT rootParameterIndex);

	//定数バッファ転送
	void TransferConstBuffer();

	//ライトの方向をセット
	void SetLightDir(const NVector3& lightdir);
	inline NVector3 GetLightDir() { return lightdir; }
	//ライトの色をセット
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor; }
	//有効フラグをセット
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//有効フラグを取得
	inline bool GetActive() { return isActive; }
};