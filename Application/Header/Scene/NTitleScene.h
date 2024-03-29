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
	std::unique_ptr<NSprite> backSprite_;	//背景スプライト

	//前景スプライト
	std::unique_ptr<NSprite> titleLogo_;						//タイトルロゴ
	std::vector<std::unique_ptr<NSprite>> aButton_{ 2 };		//AボタンUI
	NEasing::EaseTimer flashingTimer_ = 1.0f;					//点滅タイマー

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

	//その他
	NEasing::EaseTimer logoMoveTimer_ = 2.f;					//タイトルロゴ動かす用タイマー
	float logoPosY_;											//タイトルロゴY座標

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