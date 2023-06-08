#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NModel.h"
#include "NCamera.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	//オブジェクト
	NMaterial material_;				//マテリアル
	static const uint32_t maxObj = 4;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj;			//オブジェクト(定数バッファ)

	static const uint32_t maxModel = 3;	//モデル数
	std::vector<NModel> model;	//モデル情報

	//背景スプライト
	static const uint32_t maxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite[maxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t maxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite[maxForeSprite];	//前景スプライト

	NCamera camera;	//カメラ

	//オーディオ
	NAudio* audio = nullptr;
	static const uint32_t maxSoundData = 3;		//サウンドデータの最大数
	uint32_t soundData[maxSoundData] = {};	//サウンドデータ格納用

	//ライトたち
	std::unique_ptr<NDirectionalLight> directionalLight;
	std::unique_ptr<NPointLight > pointLight;
	std::unique_ptr<NSpotLight> spotLight;
	std::unique_ptr<NCircleShadow> circleShadow;

public:


public:
	//インスタンス取得
	static NGameScene* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();
	void Reset();
	//終了処理
	void Finalize();

private:


};