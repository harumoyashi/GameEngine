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

// タイトルシーン //

class NTitleScene final :
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
	NTitleScene();
	//デストラクタ
	~NTitleScene();

	//リソース読み込み
	void LoadResources();
	//初期化
	void Init() override;
	//更新
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