#pragma once
#include <stdint.h>
#include <vector>
#include "NumDrower.h"

// スコア //
// 描画機能も含む //

class Score final
{
public:
	enum class TexType
	{
		Now,
		Result,
		Top,
	};

private:
	static uint32_t sNowScore;	//現在のスコア
	static uint32_t sTopScore;	//一番高いスコア

	static std::vector<NumDrower> sScoreTex;	//スコア表示用
	static std::unique_ptr<NSprite> sTopTex;	//TOPテクスチャ

	static bool sIsAddScore;					//スコア加算してOKかフラグ

	static NEasing::EaseTimer addEaseTimer_;	//スコア加算時のイージング用タイマー

public:
	//初期化
	static void Init();
	//更新
	static void Update();
	//描画
	static void Draw();
	//ImGui描画
	static void DrawImGui();

	//記録されてたスコアを読み込み
	static void LoadScore();
	//スコアを記録
	static void SaveScore();

	//スコア加算
	static void AddScore(uint32_t score);

	// ゲッター //
	//座標取得
	static NVec2 GetPos(TexType type) { return sScoreTex[(uint32_t)type].GetPos(); }
	//大きさ取得
	static NVec2 GetSize(TexType type) { return sScoreTex[(uint32_t)type].GetSize(); }

	// セッター //
	//座標設定
	static void SetPos(const NVec2& pos, TexType type);
	//大きさ設定
	static void SetSize(const NVec2& size, TexType type);
	//色設定
	static void SetColor(const NColor& color, TexType type);
};