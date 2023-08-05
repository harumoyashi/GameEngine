#include "Score.h"

#include "NImGuiManager.h"
#include "imgui.h"

uint32_t Score::nowScore_;

void Score::Init()
{
	nowScore_ = 0;
	NumDrower::Create(5);
	NumDrower::SetPos({50.f,50.f});
	NumDrower::SetSize({50.f,50.f});
}

void Score::Update()
{
	NumDrower::Update();
}

void Score::Draw()
{
	NumDrower::Draw();
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
	NumDrower::SetNum(nowScore_);
}
