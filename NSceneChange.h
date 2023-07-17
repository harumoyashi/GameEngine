#pragma once
#include "NSprite.h"
#include "NEasing.h"

class NSceneChange
{
private:
	std::unique_ptr<NSprite> sprite_;

	bool isSceneChange_;	//シーン切り替えるかフラグ
	bool isSceneChangeNow_;	//シーン遷移中かフラグ

	NEasing::EaseTimer inTimer_ = 0.5f;		//入ってくるときのタイマー
	NEasing::EaseTimer outTimer_ = 0.5f;	//出てくときのタイマー

public:
	NSceneChange();
	static NSceneChange* GetInstance();

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画

	void Start();	//シーン遷移開始！

	// ゲッター //
	//シーン切り替えるかフラグ取得
	inline bool GetIsChange()const { return isSceneChange_; }
	//シーン遷移中かフラグ取得
	inline bool GetIsChangeNow()const { return isSceneChangeNow_; }

	// セッター //
	//シーン切り替えるかフラグ設定
	inline void SetIsChange(bool isChange) { this->isSceneChange_ = isChange; }
};