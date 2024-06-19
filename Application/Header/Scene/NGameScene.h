#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "IScene.h"
#include "NObj3d.h"
#include "NSprite.h"
#include "SimpleParticle.h"
#include "NEasing.h"

// ゲームシーン(プレイするとこ) //

//ゲームシーン内でのシーン分け
enum class SceneMode
{
	BeforeStart,	//始まる前
	Play,			//プレイ中
	Pause,			//ポーズ
	Clear,			//クリアリザルト
	Failed,			//失敗リザルト
};

enum class PauseSceneMode
{
	Retry,
	Title,
	Option
};

class NGameScene final :
	public IScene
{
private:
	//ゲーム内でのシーン状態
	static SceneMode sScene;

	//オブジェクト

	//背景スプライト

	//前景スプライト

	//ライトたち
	std::unique_ptr<NLightGroup> lightGroup_;

public:
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

	//プレイ中かどうか取得
	static bool GetIsPlay() { return sScene == SceneMode::Play; };
};