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
#include "NAudio.h"

#include "SimpleParticle.h"

class NGameScene final :
	public IScene
{
private:
	//ゲームシーン内でのシーン分け
	enum class SceneMode
	{
		Play,	//プレイ中
		Clear,	//クリアリザルト
		Faild,	//失敗リザルト
	};

	SceneMode scene = SceneMode::Play;

	//オブジェクト


	//背景スプライト
	

	//前景スプライト


	//オーディオ
	NAudio* audio_ = nullptr;

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
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