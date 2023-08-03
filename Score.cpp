#include "Score.h"

#include "NImGuiManager.h"
#include "imgui.h"

uint32_t Score::nowScore_;

void Score::Init()
{
	nowScore_ = 0;
}

void Score::Draw()
{
}

void Score::DrawImGui()
{
	ImGui::Begin("Score");
	ImGui::Text("nowScore:%d", nowScore_);
	ImGui::End();
}

void Score::AddScore(uint32_t score)
{
	nowScore_ += score;
}
