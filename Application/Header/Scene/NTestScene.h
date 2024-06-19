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

// お試しシーン //

class NTestScene final :
	public IScene
{
private:
	//オブジェクト

	//背景スプライト

	//前景スプライト

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	//コンストラクタ
	NTestScene();
	//デストラクタ
	~NTestScene();

	//リソース読み込み
	void LoadResources();
	//初期化
	void Init() override;
	//更新
	void Update() override;
	//背景スプライト描画
	void DrawBackSprite()override;
	//背景3Dオブジェクト描画
	void DrawBack3D()override;
	//3Dオブジェクト描画
	void Draw3D() override;
	//パーティクル描画
	void DrawParticle() override;
	//前景スプライト描画
	void DrawForeSprite() override;
};