#include "UI.h"

UI::UI()
{
}

UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}

void UI::Init()
{
	//読み込み～
	uiSprite_[(uint32_t)UIType::Abutton].CreateClipSprite("Abutton", { 0.f,0 }, { 192.f,192.f });
	uiSprite_[(uint32_t)UIType::AbuttonPush].CreateClipSprite("Abutton", { 192.f,0 }, { 192.f,192.f });
	uiSprite_[(uint32_t)UIType::Lstick].CreateSprite("stick");
	uiSprite_[(uint32_t)UIType::Shaft].CreateSprite("shaft");
	uiSprite_[(uint32_t)UIType::Line].CreateSprite("lineUI");
	uiSprite_[(uint32_t)UIType::Side].CreateSprite("sideUI");
	uiSprite_[(uint32_t)UIType::Wide].CreateSprite("wideUI");
	uiSprite_[(uint32_t)UIType::Clear].CreateSprite("clear");
	uiSprite_[(uint32_t)UIType::Faild].CreateSprite("faild");

	//こっち側で初期値設定するやつ
	uiSprite_[(uint32_t)UIType::Line].SetSize(250.f, 25.f);	//比率10:1
	uiSprite_[(uint32_t)UIType::Side].SetSize(250.f, 25.f);
	uiSprite_[(uint32_t)UIType::Wide].SetSize(250.f, 25.f);

	uiSprite_[(uint32_t)UIType::Line].SetPos(-150.f, 150.f);
	uiSprite_[(uint32_t)UIType::Side].SetPos(-150.f, 150.f);
	uiSprite_[(uint32_t)UIType::Wide].SetPos(-150.f, 150.f);

	//要素確保
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		easeTimer_.emplace_back();
		keepTimer_.emplace_back();
		easeBackTimer_.emplace_back();
	}

	//タイマー初期化
	easeTimer_[(uint32_t)UIType::Line] = 0.2f;
	easeTimer_[(uint32_t)UIType::Side] = 0.2f;
	easeTimer_[(uint32_t)UIType::Wide] = 0.2f;

	keepTimer_[(uint32_t)UIType::Line] = 2.f;
	keepTimer_[(uint32_t)UIType::Side] = 2.f;
	keepTimer_[(uint32_t)UIType::Wide] = 2.f;

	easeBackTimer_[(uint32_t)UIType::Line] = 0.2f;
	easeBackTimer_[(uint32_t)UIType::Side] = 0.2f;
	easeBackTimer_[(uint32_t)UIType::Wide] = 0.2f;
}

void UI::Update()
{
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		uiSprite_[i].Update();

		if (easeTimer_[i].GetStarted() && easeTimer_[i].GetEnd() == false)
		{
			easeTimer_[i].Update();

			float posX = NEasing::OutQuad(-150.f, 180.f, easeTimer_[i].GetTimeRate());
			uiSprite_[i].SetPos(posX, 150.f);

			if (easeTimer_[i].GetEnd())
			{
				keepTimer_[i].Start();
			}
		}

		if (keepTimer_[i].GetStarted() && keepTimer_[i].GetEnd() == false)
		{
			keepTimer_[i].Update();

			if (keepTimer_[i].GetEnd())
			{
				easeBackTimer_[i].Start();
			}
		}

		if (easeBackTimer_[i].GetStarted() && easeBackTimer_[i].GetEnd() == false)
		{
			easeBackTimer_[i].Update();

			float posX = NEasing::InQuad(180.f, -150.f, easeBackTimer_[i].GetTimeRate());
			uiSprite_[i].SetPos(posX, 150.f);

			if (easeBackTimer_[i].GetEnd())
			{
				easeTimer_[i].Reset();
				keepTimer_[i].Reset();
				easeBackTimer_[i].Reset();
			}
		}
	}
}

void UI::Draw(UIType uiType)
{
	//指定されたUIを描画
	uiSprite_[(uint32_t)uiType].Draw();
}

void UI::SetPos(UIType uiType, const NVec2& pos)
{
	uiSprite_[(uint32_t)uiType].SetPos(pos.x, pos.y);
}

void UI::SetSize(UIType uiType, const NVec2& size)
{
	uiSprite_[(uint32_t)uiType].SetSize(size.x, size.y);
}

void UI::SetAncorPoint(UIType uiType, const NVec2& ancorPoint)
{
	uiSprite_[(uint32_t)uiType].SetAncor(ancorPoint);
}

void UI::SetColor(UIType uiType, const NColor& color)
{
	uiSprite_[(uint32_t)uiType].color_ = color;
}

void UI::SetInvisible(UIType uiType, bool isInvisible)
{
	uiSprite_[(uint32_t)uiType].isInvisible_ = isInvisible;
}