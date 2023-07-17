#include "NSceneChange.h"

NSceneChange::NSceneChange()
{
	sprite_ = std::make_unique<NSprite>();
	sprite_->CreateSprite("mario");
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

	sprite_->SetPos(
		NWindows::GetInstance()->kWin_width * 0.5f,
		NWindows::GetInstance()->kWin_height * 0.5f);
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

		//前半が終わったなら後半スタート
		if (inTimer_.GetEnd()&& outTimer_.GetStarted() == false)
		{
			outTimer_.Start();
			isSceneChange_ = true;		//シーン切り替えてﾖｼ!!!
		}

		//終わり
		if (outTimer_.GetEnd())
		{
			isSceneChange_ = false;
			isSceneChangeNow_ = false;
		}
	}

	sprite_->Update();
}

void NSceneChange::Draw()
{
	if (isSceneChangeNow_)
	{
		sprite_->Draw();
	}
}

void NSceneChange::Start()
{
	if (isSceneChangeNow_ == false)
	{
		//ここで音鳴らしたりする//
		Init();
		isSceneChange_ = false;		//まだだ、まだ切り替えるな
		isSceneChangeNow_ = true;
	}
}
