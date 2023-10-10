#include "UI.h"

UIManager::UIManager()
{
}

UIManager* UIManager::GetInstance()
{
	static UIManager instance;
	return &instance;
}

void UIManager::Init()
{
	//読み込み～
	ui_[(uint32_t)UIType::Abutton].sprite.CreateClipSprite("Abutton", { 0.f,0.f }, { 192.f,192.f });
	ui_[(uint32_t)UIType::AbuttonPush].sprite.CreateClipSprite("Abutton", { 192.f,0.f }, { 192.f,192.f });
	ui_[(uint32_t)UIType::Lstick].sprite.CreateSprite("stick");
	ui_[(uint32_t)UIType::Shaft].sprite.CreateSprite("shaft");
	ui_[(uint32_t)UIType::Menu].sprite.CreateSprite("menuUI");
	ui_[(uint32_t)UIType::Back].sprite.CreateSprite("backUI");
	ui_[(uint32_t)UIType::Retry].sprite.CreateSprite("retryUI");
	ui_[(uint32_t)UIType::Title].sprite.CreateSprite("titleUI");
	ui_[(uint32_t)UIType::Option].sprite.CreateSprite("optionUI");
	ui_[(uint32_t)UIType::Clear].sprite.CreateSprite("clear");
	ui_[(uint32_t)UIType::Faild].sprite.CreateSprite("faild");
	ui_[(uint32_t)UIType::All].sprite.CreateSprite("allUI");
	ui_[(uint32_t)UIType::BGM].sprite.CreateSprite("bgmUI");
	ui_[(uint32_t)UIType::SE].sprite.CreateSprite("seUI");
	//弾取った時に
	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		uiBul_[i].sprite.CreateSprite();
	}
	//四角2つと点1つで構成される
	for (uint32_t i = 0; i < maxUIVol; i++)
	{
		uiVol_[i].at(0).sprite.CreateSprite("white");
		uiVol_[i].at(1).sprite.CreateSprite("white");
		uiVol_[i].at(2).sprite.CreateSprite("volumePoint");
	}

	//こっち側で初期値設定するやつ
	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		uiBul_[i].sprite.SetSize(250.f, 25.f);	//比率10:1
		uiBul_[i].sprite.SetPos(-150.f, 150.f);
		uiBul_[i].easeTimer = 0.2f;
		uiBul_[i].keepTimer = 2.f;
		uiBul_[i].easeBackTimer = 0.2f;
		uiBul_[i].startPos = { -150.f,150.f + 30.f * i };
		uiBul_[i].endPos = { 180.f,150.f + 30.f * i };
	}
}

void UIManager::Update()
{
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		ui_[i].sprite.Update();
	}
	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		uiBul_[i].sprite.Update();
	}
	for (uint32_t i = 0; i < maxUIVol; i++)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			uiVol_[i].at(j).sprite.Update();
		}
	}

	EaseTimerUpdate();
}

void UIManager::EaseTimerUpdate()
{
	for (uint32_t i = 0; i < (uint32_t)UIType::Max; i++)
	{
		if (ui_[i].easeTimer.GetStarted() && ui_[i].easeTimer.GetEnd() == false)
		{
			ui_[i].easeTimer.Update();

			float posX = NEasing::OutQuad(ui_[i].startPos.x, ui_[i].endPos.x, ui_[i].easeTimer.GetTimeRate());
			float posY = NEasing::OutQuad(ui_[i].startPos.y, ui_[i].endPos.y, ui_[i].easeTimer.GetTimeRate());
			ui_[i].sprite.SetPos(posX, posY);

			if (ui_[i].easeTimer.GetEnd())
			{
				ui_[i].keepTimer.Start();
			}
		}

		if (ui_[i].keepTimer.GetStarted() && ui_[i].keepTimer.GetEnd() == false)
		{
			ui_[i].keepTimer.Update();

			if (ui_[i].keepTimer.GetEnd())
			{
				ui_[i].easeBackTimer.Start();
			}
		}

		if (ui_[i].easeBackTimer.GetStarted() && ui_[i].easeBackTimer.GetEnd() == false)
		{
			ui_[i].easeBackTimer.Update();

			float posX = NEasing::OutQuad(ui_[i].endPos.x, ui_[i].startPos.x, ui_[i].easeBackTimer.GetTimeRate());
			float posY = NEasing::OutQuad(ui_[i].endPos.y, ui_[i].startPos.y, ui_[i].easeBackTimer.GetTimeRate());
			ui_[i].sprite.SetPos(posX, posY);

			if (ui_[i].easeBackTimer.GetEnd())
			{
				ui_[i].easeTimer.Reset();
				ui_[i].keepTimer.Reset();
				ui_[i].easeBackTimer.Reset();
			}
		}
	}

	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		if (uiBul_[i].easeTimer.GetStarted() && uiBul_[i].easeTimer.GetEnd() == false)
		{
			uiBul_[i].easeTimer.Update();

			float posX = NEasing::OutQuad(uiBul_[i].startPos.x, uiBul_[i].endPos.x, uiBul_[i].easeTimer.GetTimeRate());
			float posY = NEasing::OutQuad(uiBul_[i].startPos.y, uiBul_[i].endPos.y, uiBul_[i].easeTimer.GetTimeRate());
			uiBul_[i].sprite.SetPos(posX, posY);

			if (uiBul_[i].easeTimer.GetEnd())
			{
				uiBul_[i].keepTimer.Start();
			}
		}

		if (uiBul_[i].keepTimer.GetStarted() && uiBul_[i].keepTimer.GetEnd() == false)
		{
			uiBul_[i].keepTimer.Update();

			if (uiBul_[i].keepTimer.GetEnd())
			{
				uiBul_[i].easeBackTimer.Start();
			}
		}

		if (uiBul_[i].easeBackTimer.GetStarted() && uiBul_[i].easeBackTimer.GetEnd() == false)
		{
			uiBul_[i].easeBackTimer.Update();

			float posX = NEasing::OutQuad(uiBul_[i].endPos.x, uiBul_[i].startPos.x, uiBul_[i].easeBackTimer.GetTimeRate());
			float posY = NEasing::OutQuad(uiBul_[i].endPos.y, uiBul_[i].startPos.y, uiBul_[i].easeBackTimer.GetTimeRate());
			uiBul_[i].sprite.SetPos(posX, posY);

			if (uiBul_[i].easeBackTimer.GetEnd())
			{
				uiBul_[i].easeTimer.Reset();
				uiBul_[i].keepTimer.Reset();
				uiBul_[i].easeBackTimer.Reset();
				uiBul_[i].isActive = false;
			}
		}
	}
}

void UIManager::PlusUIBul(const std::string& texName)
{
	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		if (uiBul_[i].isActive == false)
		{
			uiBul_[i].sprite.texHandle_ =
				NTextureManager::GetInstance()->textureMap_[texName].fileName_;
			uiBul_[i].isActive = true;
			uiBul_[i].easeTimer.Start();

			break;
		}
	}
}

void UIManager::Draw(UIType uiType)
{
	//指定されたUIを描画
	if (ui_[(uint32_t)uiType].isActive)
	{
		ui_[(uint32_t)uiType].sprite.Draw();
	}
}

void UIManager::DrawUIBul()
{
	for (uint32_t i = 0; i < maxUIBul; i++)
	{
		if (uiBul_[i].isActive)
		{
			uiBul_[i].sprite.Draw();
		}
	}
}

void UIManager::SetUIVol()
{
	SetSize(UIType::All, { 150.f,50.f });
	SetPos(UIType::All,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f - 220.0f });

	SetSize(UIType::BGM, { 150.f,50.f });
	SetPos(UIType::BGM,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f - 70.f });

	SetSize(UIType::SE, { 150.f,50.f });
	SetPos(UIType::SE,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f + 80.0f });

	for (uint32_t i = 0; i < maxUIVol; i++)
	{
		for (uint32_t j = 0; j < 2; j++)
		{
			uiVol_[i].at(j).sprite.SetAncor({ 0,0.5f });
			uiVol_[i].at(j).sprite.SetSize(400.f, 10.f);
			uiVol_[i].at(j).sprite.SetPos(
				(float)NWindows::GetInstance()->kWin_width * 0.5f - uiVol_[i].at(j).sprite.GetSize().x * 0.5f,
				(float)NWindows::GetInstance()->kWin_height * 0.5f - 150.0f + i * 150.f);
		}

		uiVol_[i].at(2).sprite.SetPos(
			(float)NWindows::GetInstance()->kWin_width * 0.5f + uiVol_[i].at(0).sprite.GetSize().x * 0.5f,
			(float)NWindows::GetInstance()->kWin_height * 0.5f - 150.0f + i * 150.f);

		uiVol_[i].at(0).sprite.color_ = NColor(0.2f, 0.2f, 0.2f);	//後ろの枠は灰色に
	}
}

void UIManager::DrawUIVol()
{
	for (uint32_t i = 0; i < maxUIVol; i++)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			if (uiVol_[i].at(j).isActive)
			{
				uiVol_[i].at(j).sprite.Draw();
			}
		}
	}

	ui_[(uint32_t)UIType::All].sprite.Draw();
	ui_[(uint32_t)UIType::BGM].sprite.Draw();
	ui_[(uint32_t)UIType::SE].sprite.Draw();
}

void UIManager::SetUIVolPoint(uint32_t volType, float volume, float size)
{
	float volPosX = volume * 2.f - 1.f;
	uiVol_[volType].at(2).sprite.SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f + uiVol_[volType].at(0).sprite.GetSize().x * 0.5f * volPosX,
		(float)NWindows::GetInstance()->kWin_height * 0.5f - 150.0f + volType * 150.f);

	uiVol_[volType].at(2).sprite.SetSize(size, size);
	uiVol_[volType].at(1).sprite.SetSize(volume * 400.f, 10.f);
}

void UIManager::SetPos(UIType uiType, const NVec2& pos)
{
	ui_[(uint32_t)uiType].sprite.SetPos(pos.x, pos.y);
}

void UIManager::SetSize(UIType uiType, const NVec2& size)
{
	ui_[(uint32_t)uiType].sprite.SetSize(size.x, size.y);
}

void UIManager::SetAncorPoint(UIType uiType, const NVec2& ancorPoint)
{
	ui_[(uint32_t)uiType].sprite.SetAncor(ancorPoint);
}

void UIManager::SetColor(UIType uiType, const NColor& color)
{
	ui_[(uint32_t)uiType].sprite.color_ = color;
}

void UIManager::SetInvisible(UIType uiType, bool isInvisible)
{
	ui_[(uint32_t)uiType].sprite.isInvisible_ = isInvisible;
}

void UIManager::SetTexture(UIType uiType, const std::string& texName)
{
	ui_[(uint32_t)uiType].sprite.texHandle_ =
		NTextureManager::GetInstance()->textureMap_[texName].fileName_;
}