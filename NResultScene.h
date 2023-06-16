#pragma once
#include <vector>
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
#include "NAudio.h"
#include "NCamera.h"
#include "NCollision.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NResultScene
{
private:
	//オブジェクト
	NMaterial material_;				//マテリアル
	static const uint32_t kMaxObj = 3;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj;			//オブジェクト(定数バッファ)

	static const uint32_t kMaxModel = 2;	//モデル数
	std::vector<NModel> model;	//モデル情報

	NCollision::Sphere sphere;
	NCollision::Plane plane;
	bool isCol = false;

	//背景スプライト
	static const uint32_t kMaxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite[kMaxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t kMaxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite[kMaxForeSprite];	//前景スプライト

	NCamera camera_;	//カメラ

	//オーディオ
	NAudio* audio_ = nullptr;
	static const uint32_t kMaxSoundData = 3;		//サウンドデータの最大数
	uint32_t soundData_[kMaxSoundData] = {};	//サウンドデータ格納用

	//ライトたち
	std::unique_ptr<NDirectionalLight> directionalLight;
	std::unique_ptr<NPointLight > pointLight;
	std::unique_ptr<NSpotLight> spotLight;
	std::unique_ptr<NCircleShadow> circleShadow;

public:
	//インスタンス取得
	static NResultScene* GetInstance();

	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();
};