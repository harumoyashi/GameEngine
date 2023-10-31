#include "Score.h"

#include <fstream>
#include <string>
#include <sstream>

#include "NImGuiManager.h"
#include "imgui.h"

uint32_t Score::sNowScore;	//現在のスコア
uint32_t Score::sTopScore;	//一番高いスコア
std::vector<NumDrower> Score::sScoreTex{ 3 };	//スコア表示用
std::unique_ptr<NSprite> Score::sTopTex;		//TOPテクスチャ
bool Score::sIsAddScore;						//スコア加算してOKかフラグ
NEasing::EaseTimer Score::addEaseTimer_ = 0.1f;	//スコア加算時のイージング用タイマー

void Score::Init()
{
	LoadScore();

	sNowScore = 0;
	for (auto& scoreTex : sScoreTex)
	{
		scoreTex.Create(5);
	}

	sScoreTex[(uint32_t)TexType::Now].SetPos({ 50.f,50.f });
	sScoreTex[(uint32_t)TexType::Now].SetSize({ 45.f,50.f });
	sScoreTex[(uint32_t)TexType::Now].SetIndent(1.0f);
	sScoreTex[(uint32_t)TexType::Now].SetNum(sNowScore);
	sScoreTex[(uint32_t)TexType::Result].SetPos(
		{ NWindows::kWin_width * 0.5f - sScoreTex[(uint32_t)TexType::Result].GetSize().x * 2.5f,-500.f });
	sScoreTex[(uint32_t)TexType::Result].SetIndent(1.f);
	sScoreTex[(uint32_t)TexType::Result].SetSize({ 70.f,80.f });
	sScoreTex[(uint32_t)TexType::Result].SetNum(sNowScore);
	sScoreTex[(uint32_t)TexType::Top].SetPos(
		{ NWindows::kWin_width * 0.5f,-500.f });
	sScoreTex[(uint32_t)TexType::Top].SetIndent(1.f);
	sScoreTex[(uint32_t)TexType::Top].SetSize({ 35.f,40.f });
	sScoreTex[(uint32_t)TexType::Top].SetNum(sTopScore);

	sTopTex = std::make_unique<NSprite>();
	sTopTex->CreateSprite("top", { 0,0 });
	sTopTex->SetPos(Score::GetPos(TexType::Result).x, -500.f);
	sTopTex->SetSize(Score::GetSize(TexType::Top).x * 3.f, Score::GetSize(TexType::Top).y);

	sIsAddScore = true;
}

void Score::Update()
{
	sTopTex->SetPos(Score::GetPos(TexType::Result).x, Score::GetPos(TexType::Top).y);

	//イージングタイマー更新
	addEaseTimer_.Update();
	if (addEaseTimer_.GetEnd())
	{
		addEaseTimer_.ReverseStart();
	}
	else if (addEaseTimer_.GetReverseEnd())
	{
		addEaseTimer_.Reset();
	}

	float plusPosY = -NEasing::InQuad(addEaseTimer_.GetTimeRate() * 5.f);
	sScoreTex[(uint32_t)TexType::Now].SetPos({ 50.f ,50.f + plusPosY });

	float plusScale = NEasing::InQuad(addEaseTimer_.GetTimeRate() * 2.f);
	sScoreTex[(uint32_t)TexType::Now].SetSize({ 45.f + plusScale ,50.f + plusScale });

	for (auto& scoreTex : sScoreTex)
	{
		scoreTex.Update();
	}
	sTopTex->Update();
}

void Score::Draw()
{
	for (auto& scoreTex : sScoreTex)
	{
		scoreTex.Draw();
	}
	sTopTex->Draw();
}

void Score::DrawImGui()
{
	ImGui::Begin("Score");
	ImGui::Text("nowScore:%d", sNowScore);
	ImGui::End();
}

void Score::LoadScore()
{
	//ファイル入力処理
	std::ifstream readingFile;

	readingFile.open("./Resources/Data/score.txt");
	//ファイルオープン失敗をチェック
	if (readingFile.fail())
	{
		assert(0);
	}

	std::string line;
	getline(readingFile, line);

	sTopScore = NUtil::StringToInt(line);
}

void Score::SaveScore()
{
	//ハイスコアの更新なかったら処理スキップ
	if (sTopScore >= sNowScore)
	{
		return;
	}

	sTopScore = sNowScore;
	sScoreTex[(uint32_t)TexType::Top].SetNum(sTopScore);

	//ファイル出力処理
	std::ofstream writingFile;

	std::string filename = "";
	filename = "./Resources/Data/score.txt";

	writingFile.open(filename, std::ios::out);

	writingFile << sTopScore << std::endl;

	writingFile.close();

	sIsAddScore = false;	//これ以上スコア入らないようにする
}

void Score::AddScore(uint32_t score)
{
	if (sIsAddScore)
	{
		sNowScore += score;
		for (uint32_t i = 0; i < 2; i++)
		{
			sScoreTex[i].SetNum(sNowScore);
		}

		addEaseTimer_.Start();
	}
}

void Score::SetPos(const NVec2& pos, TexType type)
{
	sScoreTex[(uint32_t)type].SetPos(pos);
}

void Score::SetSize(const NVec2& size, TexType type)
{
	sScoreTex[(uint32_t)type].SetSize(size);
}

void Score::SetColor(const NColor& color, TexType type)
{
	sScoreTex[(uint32_t)type].SetColor(color);
}
