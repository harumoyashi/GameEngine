#include "NSceneChange.h"
#include "NAudioManager.h"

NSceneChange::NSceneChange()
{
	titleLogo_ = std::make_unique<NSprite>();
	titleLogo_->CreateSprite("logo");
	titleLogo_->SetSize(
		(float)NWindows::GetInstance()->kWin_width,
		(float)NWindows::GetInstance()->kWin_height);

	blackBack_ = std::make_unique<NSprite>();
	blackBack_->CreateSprite("white");
	blackBack_->color_ = NColor::kBlack;
	blackBack_->SetSize(
		(float)NWindows::GetInstance()->kWin_width,
		(float)NWindows::GetInstance()->kWin_height);
}

NSceneChange* NSceneChange::GetInstance()
{
	static NSceneChange instance;
	return &instance;
}

void NSceneChange::Init()
{
	isSceneChange_ = false;
	isSceneChangeNow_ = false;

	inTimer_.Reset();
	outTimer_.Reset();

	pos_ = {
		-(float)NWindows::GetInstance()->kWin_height * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f };
	titleLogo_->SetPos(pos_.x, pos_.y);
	blackBack_->SetPos(pos_.x, pos_.y);
}

void NSceneChange::Update()
{
	if (isSceneChangeNow_)
	{
		//タイマー更新
		inTimer_.Update();
		outTimer_.Update();

		//まだタイマースタートしてないなら起動
		if (inTimer_.GetStarted() == false)
		{
			inTimer_.Start();
		}

		//前半のタイマーが動いてる時
		if (inTimer_.GetRun())
		{
			pos_.x = NEasing::InOutBack(
				-(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				inTimer_.GetTimeRate());
		}

		//前半が終わったなら後半スタート
		if (inTimer_.GetEnd() && outTimer_.GetStarted() == false)
		{
			outTimer_.Start();
			isSceneChange_ = true;		//シーン切り替えてﾖｼ!!!
		}

		//後半のタイマーが動いてる時
		if (outTimer_.GetRun())
		{
			pos_.x = NEasing::InQuad(
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_width * 1.5f,
				outTimer_.GetTimeRate());
		}

		//終わり
		if (outTimer_.GetEnd())
		{
			isSceneChange_ = false;
			isSceneChangeNow_ = false;
		}
	}

	titleLogo_->SetPos(pos_.x, pos_.y);
	blackBack_->SetPos(pos_.x, pos_.y);
	titleLogo_->Update();
	blackBack_->Update();
}

void NSceneChange::Draw()
{
	if (isSceneChangeNow_)
	{
		blackBack_->Draw();
		titleLogo_->Draw();
	}
}

void NSceneChange::Start()
{
	if (isSceneChangeNow_ == false)
	{
		NAudioManager::Play("sceneChangeSE");
		Init();
		isSceneChange_ = false;		//まだだ、まだ切り替えるな
		isSceneChangeNow_ = true;
	}
}
