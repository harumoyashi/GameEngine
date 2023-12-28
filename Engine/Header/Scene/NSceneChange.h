#pragma once
#include "NSprite.h"
#include "NEasing.h"

// シーン遷移 //

class NSceneChange
{
private:
	std::unique_ptr<NSprite> titleLogo_;	//タイトルロゴ
	std::unique_ptr<NSprite> blackBack_;	//背景の黒い奴

	bool isSceneChange_;	//シーン切り替えるかフラグ
	bool isSceneChangeNow_;	//シーン遷移中かフラグ

	NEasing::EaseTimer inTimer_ = 0.5f;		//入ってくるときのタイマー
	NEasing::EaseTimer outTimer_ = 0.5f;	//出てくときのタイマー

	NVec2 pos_;	//暗幕の座標

public:
	//コンストラクタ
	NSceneChange();
	//シングルトンインスタンス取得
	static NSceneChange* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	//シーン遷移開始！
	void Start();

	// ゲッター //
	//シーン切り替えるかフラグ取得
	bool GetIsChange()const { return isSceneChange_; }
	//シーン遷移中かフラグ取得
	bool GetIsChangeNow()const { return isSceneChangeNow_; }

	// セッター //
	//シーン切り替えるかフラグ設定
	void SetIsChange(bool isChange) { this->isSceneChange_ = isChange; }

private:
	//コピー禁止
	NSceneChange(const NSceneChange&) = delete;
	//デストラクタ
	~NSceneChange() = default;
	//コピー禁止
	NSceneChange& operator=(const NSceneChange&) = delete;
};