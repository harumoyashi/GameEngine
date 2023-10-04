#include "Score.h"

#include <fstream>
#include <string>
#include <sstream>

#include "NImGuiManager.h"
#include "imgui.h"

uint32_t Score::sNowScore;	//���݂̃X�R�A
uint32_t Score::sTopScore;	//��ԍ����X�R�A
std::vector<NumDrower> Score::sScoreTex{ 3 };	//�X�R�A�\���p
std::unique_ptr<NSprite> Score::sTopTex;		//TOP�e�N�X�`��
bool Score::sIsAddScore;	//�X�R�A���Z����OK���t���O

void Score::Init()
{
	LoadScore();

	sNowScore = 0;
	for (size_t i = 0; i < sScoreTex.size(); i++)
	{
		sScoreTex[i].Create(5);
	}

	sScoreTex[(uint32_t)TexType::Now].SetPos({ 50.f,50.f });
	sScoreTex[(uint32_t)TexType::Now].SetSize({ 45.f,50.f });
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

	for (size_t i = 0; i < sScoreTex.size(); i++)
	{
		sScoreTex[i].Update();
	}
	sTopTex->Update();
}

void Score::Draw()
{
	for (size_t i = 0; i < sScoreTex.size(); i++)
	{
		sScoreTex[i].Draw();
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
	//�t�@�C�����͏���
	std::ifstream readingFile;

	readingFile.open("./Resources/Data/score.txt");
	//�t�@�C���I�[�v�����s���`�F�b�N
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
	//�n�C�X�R�A�̍X�V�Ȃ������珈���X�L�b�v
	if (sTopScore >= sNowScore)
	{
		return;
	}

	sTopScore = sNowScore;
	sScoreTex[(uint32_t)TexType::Top].SetNum(sTopScore);

	//�t�@�C���o�͏���
	std::ofstream writingFile;

	std::string filename = "";
	filename = "./Resources/Data/score.txt";

	writingFile.open(filename, std::ios::out);

	writingFile << sTopScore << std::endl;

	writingFile.close();

	sIsAddScore = false;	//����ȏ�X�R�A����Ȃ��悤�ɂ���
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
