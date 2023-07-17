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
		//�^�C�}�[�X�V
		inTimer_.Update();
		outTimer_.Update();

		//�܂��^�C�}�[�X�^�[�g���ĂȂ��Ȃ�N��
		if (inTimer_.GetStarted() == false)
		{
			inTimer_.Start();
		}

		//�O�����I������Ȃ�㔼�X�^�[�g
		if (inTimer_.GetEnd()&& outTimer_.GetStarted() == false)
		{
			outTimer_.Start();
			isSceneChange_ = true;		//�V�[���؂�ւ���ּ!!!
		}

		//�I���
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
		//�����ŉ��炵���肷��//
		Init();
		isSceneChange_ = false;		//�܂����A�܂��؂�ւ����
		isSceneChangeNow_ = true;
	}
}
