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
	static uint32_t nowScore_;	//現在のスコア
	static uint32_t topScore_;	//一番高いスコア

	static std::vector<NumDrower> scoreTex_;	//スコア表示用
	static std::unique_ptr<NSprite> topTex_;	//TOPテクスチャ

public:
	static void Create();
	static void Init();
	static void Update();
	static void Draw();
	static void DrawImGui();

	static void AddScore(uint32_t score);

	// ゲッター //
	static NVector2 GetPos(TexType type) { return scoreTex_[(uint32_t)type].GetPos(); }
	static NVector2 GetSize(TexType type) { return scoreTex_[(uint32_t)type].GetSize(); }

	// セッター //
	static void SetPos(const NVector2& pos, TexType type);
	static void SetSize(const NVector2& size, TexType type);
	static void SetColor(const NColor& color, TexType type);
};