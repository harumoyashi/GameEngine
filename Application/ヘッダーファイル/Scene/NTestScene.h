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
#include "NAssimpModel.h"

class NTestScene final :
	public IScene
{
private:
	//オブジェクト
	static const uint32_t kMaxObj = 3;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj_;			//オブジェクト(定数バッファ)
	std::vector<std::unique_ptr<NObj3d>> levelDataobj_;	//レベルデータから読み込んだオブジェクト

	bool isCol_ = false;

	//背景スプライト
	static const uint32_t kMaxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t kMaxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//前景スプライト

	//NAssimpModel assimpModel_;

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