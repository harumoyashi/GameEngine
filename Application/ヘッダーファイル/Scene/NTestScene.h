#pragma once
#include <vector>
#include <wrl.h>

#include "IScene.h"
#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NCollision.h"
#include "NLightGroup.h"
#include "NEasing.h"
#include "NConstBuff.h"
#include "NLevelDataLoader.h"

class NTestScene final :
	public IScene
{
private:
	//オブジェクト
	std::unique_ptr<NObj3d> obj_;
	std::vector<std::unique_ptr<NObj3d>> levelDataobj_;	//レベルデータから読み込んだオブジェクト

	bool isCol_ = false;

	//背景スプライト
	std::unique_ptr<NSprite> backSprite_;	//背景スプライト

	//前景スプライト
	std::unique_ptr<NSprite> foreSprite_;	//前景スプライト

	//blenderのレベルデータ
	std::unique_ptr<LevelData> levelData_;

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	NTestScene();
	~NTestScene();

	void LoadResources();
	void Init() override;
	void Update() override;
	//背景スプライト
	void DrawBackSprite()override;
	//背景3Dオブジェクト
	void DrawBack3D()override;
	//3Dオブジェクト
	void Draw3D() override;
	//パーティクル
	void DrawParticle() override;
	//前景スプライト
	void DrawForeSprite() override;
};