#include "NSceneChange.h"
#include "NAudioManager.h"

NSceneChange::NSceneChange()
{
	sprite_ = std::make_unique<NSprite>();
	sprite_->CreateSprite("sceneChange");
	sprite_->SetSize(
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
	sprite_->SetPos(pos_.x, pos_.y);
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

		//�O���̃^�C�}�[�������Ă鎞
		if (inTimer_.GetRun())
		{
			pos_.x = NEasing::InOutBack(
				-(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				inTimer_.GetTimeRate());
		}

		//�O�����I������Ȃ�㔼�X�^�[�g
		if (inTimer_.GetEnd() && outTimer_.GetStarted() == false)
		{
			outTimer_.Start();
			isSceneChange_ = true;		//�V�[���؂�ւ���ּ!!!
		}

		//�㔼�̃^�C�}�[�������Ă鎞
		if (outTimer_.GetRun())
		{
			pos_.x = NEasing::InQuad(
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_width * 1.5f,
				outTimer_.GetTimeRate());
		}

		//�I���
		if (outTimer_.GetEnd())
		{
			isSceneChange_ = false;
			isSceneChangeNow_ = false;
		}
	}

	sprite_->SetPos(pos_.x, pos_.y);
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
		NAudioManager::Play("sceneChangeSE");
		Init();
		isSceneChange_ = false;		//�܂����A�܂��؂�ւ����
		isSceneChangeNow_ = true;
	}
}
