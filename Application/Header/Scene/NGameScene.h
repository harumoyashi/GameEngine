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
	Faild,			//失敗リザルト
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
	std::unique_ptr<NSprite> backSprite_;	//背景スプライト

	//前景スプライト
	std::unique_ptr<NSprite> darken_;						//ポーズ画面の暗幕的な
	std::array<std::unique_ptr<NSprite>,2> movieDarken_;	//ムービーシーンの暗幕的な

	//UI用の変数
	NEasing::EaseTimer beforeStartTimer_ = 2.0f;	//始まる前の演出タイマー
	NEasing::EaseTimer stickRotTimer_ = 2.0f;		//Lスティックぐるぐるタイマー
	float slidePos_ = 0.0f;							//横に掃けてく時のスライド位置
	NEasing::EaseTimer slideTimer_ = 0.1f;			//スライド時のイージング用
	NEasing::EaseTimer flashingTimer_ = 1.0f;		//点滅タイマー

	//ポーズ画面用
	PauseSceneMode pauseScene_ = PauseSceneMode::Retry;	//ポーズシーンでどれ選ばれてるか
	bool isOption_ = false;								//オプション画面か
	uint32_t volumeType_ = 0;							//選択してる音量の種類(0:全体,1:BGM,2:SE)
	std::vector<float> volume_;							//音量群(0.0f~1.0f)
	NEasing::EaseTimer volEaseTimer_ = 1.0f;			//音量が増える量を徐々に上げるためのイージングタイマー

	//無敵演出用
	std::unique_ptr<NSprite> mutekiTex_;				//無敵テキスト
	NVec2 mutekiTexStartPos;
	NVec2 mutekiTexEndPos;
	NVec2 mutekiTexNowPos;
	NEasing::EaseTimer mutekiInTimer_ = 0.3f;			//無敵テキスト出現のためのイージングタイマー
	NEasing::EaseTimer mutekiOutTimer_ = 0.3f;			//無敵テキスト掃けるのためのイージングタイマー

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