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
	//スプライト生成
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
	ui_[(uint32_t)UIType::Failed].sprite.CreateSprite("failed");
	ui_[(uint32_t)UIType::All].sprite.CreateSprite("allUI");
	ui_[(uint32_t)UIType::BGM].sprite.CreateSprite("bgmUI");
	ui_[(uint32_t)UIType::SE].sprite.CreateSprite("seUI");
	//弾取った時のやつは動的に割り当てるからとりま空
	for (uint32_t i = 0; i < kMaxUIBul; i++)
	{
		uiBul_[i].sprite.CreateSprite();
	}
	//音量調節UIは四角2つと点1つで構成される
	for (uint32_t i = 0; i < kMaxUIVol; i++)
	{
		uiVol_[i].at(0).sprite.CreateSprite("white");
		uiVol_[i].at(1).sprite.CreateSprite("white");
		uiVol_[i].at(2).sprite.CreateSprite("volumePoint");
	}

	//こっち側で初期値設定するやつ
	for (uint32_t i = 0; i < kMaxUIBul; i++)
	{
		uiBul_[i].sprite.SetSize(250.f, 25.f);	//比率10:1
		uiBul_[i].sprite.SetPos(-150.f, 150.f);
		uiBul_[i].easeTimer = 0.2f;
		uiBul_[i].keepTimer = 2.f;
		uiBul_[i].easeBackTimer = 0.2f;
		uiBul_[i].startPos = { -150.f,150.f + 30.f * i };
		uiBul_[i].endPos = { 180.f,150.f + 30.f * i };
		uiBul_[i].sprite.isInvisible_ = true;	//とりあえず出さない
	}
}

void UIManager::Update()
{
	//それぞれのスプライト更新忘れずに
	for (auto& ui : ui_)
	{
		ui.sprite.Update();
	}
	for (auto& uiBul : uiBul_)
	{
		uiBul.sprite.Update();
	}
	for (auto& uiVol : uiVol_)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			uiVol.at(i).sprite.Update();
		}
	}
	//イージングタイマーの更新も
	EaseTimerUpdate();
}

void UIManager::EaseTimerUpdate()
{
	for (auto& ui : ui_)
	{
		//スタートしてて、終了してなければ
		if (ui.easeTimer.GetStarted() && ui.easeTimer.GetEnd() == false)
		{
			ui.easeTimer.Update();

			//設定された始点から終点に移動
			float posX = NEasing::OutQuad(ui.startPos.x, ui.endPos.x, ui.easeTimer.GetTimeRate());
			float posY = NEasing::OutQuad(ui.startPos.y, ui.endPos.y, ui.easeTimer.GetTimeRate());
			ui.sprite.SetPos(posX, posY);

			if (ui.easeTimer.GetEnd())	//行きが終わったら止まる時間も設ける
			{
				ui.keepTimer.Start();
			}
		}

		//ここは待機してるだけ
		if (ui.keepTimer.GetStarted() && ui.keepTimer.GetEnd() == false)
		{
			ui.keepTimer.Update();

			if (ui.keepTimer.GetEnd())	//待機時間終わったら戻る
			{
				ui.easeBackTimer.Start();
			}
		}

		//戻りの処理
		if (ui.easeBackTimer.GetStarted() && ui.easeBackTimer.GetEnd() == false)
		{
			ui.easeBackTimer.Update();

			//今度は終点から始点まで移動
			float posX = NEasing::OutQuad(ui.endPos.x, ui.startPos.x, ui.easeBackTimer.GetTimeRate());
			float posY = NEasing::OutQuad(ui.endPos.y, ui.startPos.y, ui.easeBackTimer.GetTimeRate());
			ui.sprite.SetPos(posX, posY);

			//戻り切ったら3つのタイマーをリセット
			if (ui.easeBackTimer.GetEnd())
			{
				ui.easeTimer.Reset();
				ui.keepTimer.Reset();
				ui.easeBackTimer.Reset();
			}
		}
	}

	//弾取った時UIも変わらず
	for (auto& uiBul : uiBul_)
	{
		if (uiBul.easeTimer.GetStarted() && uiBul.easeTimer.GetEnd() == false)
		{
			uiBul.easeTimer.Update();

			float posX = NEasing::OutQuad(uiBul.startPos.x, uiBul.endPos.x, uiBul.easeTimer.GetTimeRate());
			float posY = NEasing::OutQuad(uiBul.startPos.y, uiBul.endPos.y, uiBul.easeTimer.GetTimeRate());
			uiBul.sprite.SetPos(posX, posY);

			if (uiBul.easeTimer.GetEnd())
			{
				uiBul.keepTimer.Start();
			}
		}

		if (uiBul.keepTimer.GetStarted() && uiBul.keepTimer.GetEnd() == false)
		{
			uiBul.keepTimer.Update();

			if (uiBul.keepTimer.GetEnd())
			{
				uiBul.easeBackTimer.Start();
			}
		}

		if (uiBul.easeBackTimer.GetStarted() && uiBul.easeBackTimer.GetEnd() == false)
		{
			uiBul.easeBackTimer.Update();

			float posX = NEasing::OutQuad(uiBul.endPos.x, uiBul.startPos.x, uiBul.easeBackTimer.GetTimeRate());
			float posY = NEasing::OutQuad(uiBul.endPos.y, uiBul.startPos.y, uiBul.easeBackTimer.GetTimeRate());
			uiBul.sprite.SetPos(posX, posY);

			if (uiBul.easeBackTimer.GetEnd())
			{
				uiBul.easeTimer.Reset();
				uiBul.keepTimer.Reset();
				uiBul.easeBackTimer.Reset();
				uiBul.sprite.isInvisible_ = true;
			}
		}
	}
}

void UIManager::PlusUIBul(const std::string& texName)
{
	for (auto& uiBul : uiBul_)
	{
		if (uiBul.sprite.isInvisible_ == true)	//見えない状態なら
		{
			//指定されたテクスチャを割り当てて見えるようにして、イージングタイマー起動
			uiBul.sprite.texHandle_ =
				NTextureManager::GetInstance()->textureMap_[texName].fileName_;
			uiBul.sprite.isInvisible_ = false;
			uiBul.easeTimer.Start();

			break;
		}
	}
}

void UIManager::Draw(UIType uiType)
{
	//指定されたUIを描画
	ui_[(uint32_t)uiType].sprite.Draw();
}

void UIManager::DrawUIBul()
{
	for (auto& uiBul : uiBul_)
	{
		uiBul.sprite.Draw();
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

	for (uint32_t i = 0; i < kMaxUIVol; i++)
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
	for (auto& uiVol : uiVol_)
	{
		for (uint32_t j = 0; j < 3; j++)
		{
			uiVol.at(j).sprite.Draw();
		}
	}

	ui_[(uint32_t)UIType::All].sprite.Draw();
	ui_[(uint32_t)UIType::BGM].sprite.Draw();
	ui_[(uint32_t)UIType::SE].sprite.Draw();
}

void UIManager::SetUIVolPoint(uint32_t volType, float volume, float size)
{
	//正の値と負の値が半々になるように
	float volPosX = volume * 2.f - 1.f;

	//点の位置を指定された音量の位置に合わせる
	uiVol_[volType].at(2).sprite.SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f + uiVol_[volType].at(0).sprite.GetSize().x * 0.5f * volPosX,
		(float)NWindows::GetInstance()->kWin_height * 0.5f - 150.0f + volType * 150.f);
	//点は指定されたサイズに
	uiVol_[volType].at(2).sprite.SetSize(size, size);
	//音量に合わせて音量バーのサイズを変える
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