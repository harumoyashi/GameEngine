#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NConstBuff.h"

// 平行光源 //

class NDirectionalLight final
{
private://静的メンバ変数
	NVec3 lightdir_ = { 1,0,0 };	 // ライト光線方向
	NVec3 lightcolor_ = NVec3::one; // ライト色

	//ダーティフラグ
	bool isDirty_ = false;
	//有効フラグ
	bool isActive_ = false;

public:
	//定数バッファ
	std::unique_ptr<NConstBuff<ConstBuffDataLight>> cbLight;

public: //メンバ関数
	//コンストラクタ
	NDirectionalLight();
	//デストラクタ
	~NDirectionalLight();

	//初期化
	void Init();

	//ライトの方向をセット
	void SetLightDir(const NVec3& lightdir);
	const NVec3& GetLightDir()const { return lightdir_; }
	//ライトの色をセット
	void SetLightColor(const NVec3& lightcolor);
	const NVec3& GetLightColor()const { return lightcolor_; }
	//有効フラグをセット
	void SetActive(bool isActive) { isActive_ = isActive; }
	//有効フラグを取得
	bool GetActive()const { return isActive_; }
};