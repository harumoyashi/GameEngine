#pragma once
#include <stdint.h>
#include <vector>
#include "NumDrower.h"

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

	static bool sIsAddScore;	//スコア加算してOKかフラグ

public:
	static void Create();
	static void Init();
	static void Update();
	static void Draw();
	static void DrawImGui();

	static void LoadScore();
	static void SaveScore();

	static void AddScore(uint32_t score);

	// ゲッター //
	static NVec2 GetPos(TexType type) { return sScoreTex[(uint32_t)type].GetPos(); }
	static NVec2 GetSize(TexType type) { return sScoreTex[(uint32_t)type].GetSize(); }

	// セッター //
	static void SetPos(const NVec2& pos, TexType type);
	static void SetSize(const NVec2& size, TexType type);
	static void SetColor(const NColor& color, TexType type);
};