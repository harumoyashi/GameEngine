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

	//前景スプライトの種類
	enum class FSpriteType
	{
		Shaft,
		LStick,
		Abutton,
		AbuttonPush,
		Clear,
		Faild,

		MaxForeSprite
	};

	SceneMode scene = SceneMode::Play;

	//オブジェクト


	//背景スプライト
	std::unique_ptr<NSprite> backSprite_;	//背景スプライト

	//前景スプライト
	std::vector<std::unique_ptr<NSprite>> foreSprite_{ (uint32_t)FSpriteType::MaxForeSprite };		//LスティックUI
	float slidePos = 0.0f;							//横に掃けてく時のスライド位置
	NEasing::EaseTimer slideTimer = 0.1f;			//スライド時のイージング用
	NEasing::EaseTimer flashingTimer_ = 1.0f;		//点滅タイマー

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
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