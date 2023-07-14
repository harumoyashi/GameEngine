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
#include "NTimer.h"
#include "NAssimpModel.h"
#include "NConstBuff.h"
#include "NLevelDataLoader.h"

class NTitleScene final :
	public IScene
{
private:
	//オブジェクト
	NMaterial material_;				//マテリアル
	static const uint32_t kMaxObj = 3;	//オブジェクト数
	std::vector<std::unique_ptr<NObj3d>> obj_;	//オブジェクト
	std::vector<std::unique_ptr<NObj3d>> levelDataobj_;	//レベルデータから読み込んだオブジェクト

	Sphere sphere_;
	Plane plane_;
	bool isCol_ = false;

	//背景スプライト
	static const uint32_t kMaxBackSprite = 3;	//背景スプライト数
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//背景スプライト

	//前景スプライト
	static const uint32_t kMaxForeSprite = 3;	//前景スプライト数
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//前景スプライト

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

	NTimer timer_;

	std::unique_ptr<LevelData> levelData_;

	NAssimpModel assimpModel_;

public:
	//インスタンス取得
	static NTitleScene* GetInstance();

	NTitleScene();
	~NTitleScene();

	void LoadResources();
	void Init() override;
	void Update() override;
	//背景3Dオブジェクト
	void DrawBack3D()override;
	//背景スプライト
	void DrawBackSprite()override;
	//3Dオブジェクト
	void Draw3D() override;
	//パーティクル
	void DrawParticle() override;
	//前景スプライト
	void DrawForeSprite() override;
};