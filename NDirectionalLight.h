#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NDirectionalLight final
{
private://静的メンバ変数
	NVector3 lightdir_ = { 1,0,0 };	 // ライト光線方向
	NVector3 lightcolor_ = { 1,1,1 }; // ライト色

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataLight>> cbLight;

public: //メンバ関数
	NDirectionalLight();
	~NDirectionalLight();

	//初期化
	void Init();

	//ライトの方向をセット
	void SetLightDir(const NVector3& lightdir);
	const NVector3& GetLightDir()const { return lightdir_; }
	//ライトの色をセット
	void SetLightColor(const NVector3& lightcolor);
	const NVector3& GetLightColor()const { return lightcolor_; }
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }
};