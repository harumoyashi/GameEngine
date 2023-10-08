#pragma once
#include <memory>
#include <vector>
#include "NSprite.h"
#include "NEasing.h"

enum class UIType
{
	Abutton,	//Aボタン
	AbuttonPush,//Aボタン押されたやつ
	Lstick,		//Lスティック
	Shaft,		//スティックの軸
	Menu,		//メニュー
	Back,		//メニューからもどる
	Retry,		//リトライ
	Title,		//タイトルへ
	Option,		//オプション
	Clear,		//クリア
	Faild,		//失敗
	All,		//全部の音量
	BGM,		//BGMの音量
	SE,			//SEの音量

	Max			//最大数(for文とか回す用)
};

struct UI
{
	NSprite sprite;						//UI描画用スプライト群
	NEasing::EaseTimer easeTimer;		//UI用イージングタイマー群
	NEasing::EaseTimer keepTimer;		//UI用待機タイマー群
	NEasing::EaseTimer easeBackTimer;	//UI用イージングバックタイマー群
	NVec2 startPos;						//始点座標
	NVec2 endPos;						//終点座標
	bool isActive;						//有効フラグ
};

class UIManager
{
private:
	std::vector<UI> ui_{ ((uint32_t)UIType::Max) };	//UI群
	const uint32_t maxUIBul = 3;
	std::vector<UI> uiBul_{ (maxUIBul) };	//弾取った時のUI群
	const uint32_t maxUIVol = 3;
	std::vector<UI> uiVol_{(maxUIVol)};		//音量調節UI群

public:
	UIManager();
	static UIManager* GetInstance();

	void Init();
	void Update();
	void EaseTimerUpdate();

	//弾取った時UI追加
	void PlusUIBul(const std::string& texName);

	//指定されたUIを描画
	void Draw(UIType uiType);
	//弾とった時のUI描画
	void DrawUIBul();

	//指定されたUIのタイマースタート
	void StartEaseTimer(UIType uiType) { ui_[(uint32_t)uiType].easeTimer.Start(); }

	//指定されたUIの描画座標設定
	void SetPos(UIType uiType, const NVec2& pos);
	//指定されたUIの描画サイズ設定
	void SetSize(UIType uiType, const NVec2& size);
	//指定されたUIの描画基準点設定
	void SetAncorPoint(UIType uiType, const NVec2& ancorPoint);
	//指定されたUIの描画色設定
	void SetColor(UIType uiType, const NColor& color);
	//指定されたUIの非表示フラグ設定
	void SetInvisible(UIType uiType, bool isInvisible);
	//指定されたUIのテクスチャ設定
	void SetTexture(UIType uiType, const std::string& texName);
};