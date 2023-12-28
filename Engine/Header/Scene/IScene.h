#pragma once
#include <string>

// シーンのひな形 //

class IScene
{
public:
	//コンストラクタ
	IScene() {};
	//デストラクタ
	virtual ~IScene() = default;

	//リソース読み込み
	virtual void LoadResources() = 0;
	//初期化
	virtual void Init() = 0;
	//更新
	virtual void Update() = 0;
	//背景スプライト描画
	virtual void DrawBackSprite() = 0;
	//背景オブジェクト描画
	virtual void DrawBack3D() = 0;
	//前景オブジェクト描画
	virtual void Draw3D() = 0;
	//パーティクル描画
	virtual void DrawParticle() = 0;
	//前景スプライト描画
	virtual void DrawForeSprite() = 0;

	//現在のシーンがどこか判断する変数(変数自体は個々のシーン内で書き換える)
	std::string sceneID = "";
};