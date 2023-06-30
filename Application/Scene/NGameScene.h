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
	

	//前景スプライト

	NCamera camera_;	//カメラ

	//オーディオ
	NAudio* audio_ = nullptr;

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

	//動きの更新管理フラグ
	bool isMoveUpdate_ = true;

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