#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "IScene.h"
#include "NObj3d.h"
#include "NSprite.h"
#include "SimpleParticle.h"
#include "NEasing.h"

//ゲームシーン内でのシーン分け
enum class SceneMode
{
	BeforeStart,	//始まる前
	Play,			//プレイ中
	Pause,			//ポーズ
	Clear,			//クリアリザルト
	Faild,			//失敗リザルト
};

class NGameScene final :
	public IScene
{
private:
	//ゲーム内でのシーン状態
	static SceneMode sScene;

	//オブジェクト

	//背景スプライト
	std::unique_ptr<NSprite> backSprite_;	//背景スプライト

	//前景スプライト

	//UI用の変数
	NEasing::EaseTimer beforeStartTimer_ = 2.0f;	//始まる前の演出タイマー
	NEasing::EaseTimer stickRotTimer_ = 2.0f;		//Lスティックぐるぐるタイマー
	float slidePos_ = 0.0f;							//横に掃けてく時のスライド位置
	NEasing::EaseTimer slideTimer_ = 0.1f;			//スライド時のイージング用
	NEasing::EaseTimer flashingTimer_ = 1.0f;		//点滅タイマー

	//ポーズ画面用
	bool isRetry_ = true;

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

	//プレイ中かどうか取得
	static bool GetIsPlay() { return sScene == SceneMode::Play; }
};