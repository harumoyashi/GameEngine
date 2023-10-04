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
	static uint32_t sNowScore;	//���݂̃X�R�A
	static uint32_t sTopScore;	//��ԍ����X�R�A

	static std::vector<NumDrower> sScoreTex;	//�X�R�A�\���p
	static std::unique_ptr<NSprite> sTopTex;	//TOP�e�N�X�`��

	static bool sIsAddScore;	//�X�R�A���Z����OK���t���O

public:
	static void Create();
	static void Init();
	static void Update();
	static void Draw();
	static void DrawImGui();

	static void LoadScore();
	static void SaveScore();

	static void AddScore(uint32_t score);

	// �Q�b�^�[ //
	static NVec2 GetPos(TexType type) { return sScoreTex[(uint32_t)type].GetPos(); }
	static NVec2 GetSize(TexType type) { return sScoreTex[(uint32_t)type].GetSize(); }

	// �Z�b�^�[ //
	static void SetPos(const NVec2& pos, TexType type);
	static void SetSize(const NVec2& size, TexType type);
	static void SetColor(const NColor& color, TexType type);
};