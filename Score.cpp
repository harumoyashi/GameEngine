#include "Score.h"

#include "NImGuiManager.h"
#include "imgui.h"

uint32_t Score::nowScore_;
uint32_t Score::topScore_;
std::vector<NumDrower> Score::scoreTex_{ 3 };
std::unique_ptr<NSprite> Score::topTex_;

void Score::Init()
{
	nowScore_ = 0;
	for (size_t i = 0; i < scoreTex_.size(); i++)
	{
		scoreTex_[i].Create(5);
	}

	scoreTex_[(uint32_t)TexType::Now].SetPos({ 50.f,50.f });
	scoreTex_[(uint32_t)TexType::Now].SetSize({ 50.f,50.f });
	scoreTex_[(uint32_t)TexType::Now].SetNum(nowScore_);
	scoreTex_[(uint32_t)TexType::Result].SetPos(
		{ NWindows::kWin_width * 0.5f - scoreTex_[(uint32_t)TexType::Result].GetSize().x * 2.5f,-500.f });
	scoreTex_[(uint32_t)TexType::Result].SetSize({ 80.f,80.f });
	scoreTex_[(uint32_t)TexType::Result].SetNum(nowScore_);
	scoreTex_[(uint32_t)TexType::Top].SetPos(
		{ NWindows::kWin_width * 0.5f,-500.f });
	scoreTex_[(uint32_t)TexType::Top].SetSize({ 40.f,40.f });
	scoreTex_[(uint32_t)TexType::Top].SetNum(topScore_);

	topTex_ = std::make_unique<NSprite>();
	topTex_->CreateSprite("top",{0,0});
	topTex_->SetPos(Score::GetPos(TexType::Result).x, -500.f);
	topTex_->SetSize(Score::GetSize(TexType::Top).x * 3.f, Score::GetSize(TexType::Top).y);
}

void Score::Update()
{
	//ハイスコアの更新あったら
	if (topScore_ < nowScore_)
	{
		topScore_ = nowScore_;
		scoreTex_[(uint32_t)TexType::Top].SetNum(topScore_);
	}
	topTex_->SetPos(Score::GetPos(TexType::Result).x, Score::GetPos(TexType::Top).y);

	for (size_t i = 0; i < scoreTex_.size(); i++)
	{
		scoreTex_[i].Update();
	}
	topTex_->Update();
}

void Score::Draw()
{
	for (size_t i = 0; i < scoreTex_.size(); i++)
	{
		scoreTex_[i].Draw();
	}
	topTex_->Draw();
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
	for (uint32_t i = 0; i < 2; i++)
	{
		scoreTex_[i].SetNum(nowScore_);
	}
}

void Score::SetPos(const NVec2& pos, TexType type)
{
	scoreTex_[(uint32_t)type].SetPos(pos);
}

void Score::SetSize(const NVec2& size, TexType type)
{
	scoreTex_[(uint32_t)type].SetSize(size);
}

void Score::SetColor(const NColor& color, TexType type)
{
	scoreTex_[(uint32_t)type].SetColor(color);
}
