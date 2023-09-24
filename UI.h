#pragma once
#include <memory>
#include <vector>
#include "NSprite.h"

enum class UIType
{
	Abutton,	//Aボタン
	AbuttonPush,//Aボタン押されたやつ
	Lstick,		//Lスティック
	Shaft,		//スティックの軸
	Line,		//ライン弾取った時UI
	Side,		//サイド弾取った時UI
	Wide,		//ワイド弾取った時UI
	Clear,		//クリア
	Faild,		//失敗

	Max			//最大数(for文とか回す用)
};

class UI
{
private:
	std::vector<NSprite> uiSprite_{ ((uint32_t)UIType::Max) };		//UI描画用スプライト群

public:
	UI();
	static UI* GetInstance();

	void Init();
	void Update();

	//指定されたUIを描画
	void Draw(UIType uiType);

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
};