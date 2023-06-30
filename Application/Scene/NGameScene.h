#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "IScene.h"
#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NModel.h"
#include "NCamera.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene:
	public IScene
{
private:
	//オブジェクト

	//背景スプライト
	static const uint32_t kMaxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t kMaxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//前景スプライト

	NCamera camera_;	//カメラ

	//オーディオ
	NAudio* audio_ = nullptr;

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:


public:
	//インスタンス取得
	static NGameScene* GetInstance();

	void LoadResources();
	void Init() override;
	void Update() override;
	//背景スプライト
	void DrawBackSprite()override;
	//3Dオブジェクト
	void Draw3D() override;
	//前景スプライト
	void DrawForeSprite() override;

private:


};