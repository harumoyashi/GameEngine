#pragma once
#include <stdint.h>

class Score
{
private:
	static uint32_t nowScore_;
	uint32_t topScore_;

public:
	static void Init();
	static void Draw();
	static void DrawImGui();

	static void AddScore(uint32_t score);
};