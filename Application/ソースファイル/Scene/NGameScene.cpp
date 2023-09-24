#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NAudioManager.h"
#include "NParticleManager.h"
#include "NCollisionManager.h"
#include "IPostEffect.h"
#include "Bloom.h"

#include "Player.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Field.h"
#include "Wave.h"
#include "Score.h"
#include "UI.h"

SceneMode NGameScene::scene = SceneMode::Play;

void NGameScene::LoadResources()
{
}

void NGameScene::Init()
{
#pragma region	�I�[�f�B�I������
	NAudio::GetInstance()->Init();
	NAudioManager::Play("playBGM", true, 0.2f);
#pragma endregion
#pragma region	�J����������
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	NParticleManager::GetInstance()->Init();
	NCollisionManager::GetInstance()->Init();
	Player::GetInstance()->Init();
	Player::GetInstance()->SetIsElapseAnime(true);
	BulletManager::GetInstance()->Init();
	EnemyManager::GetInstance()->Init();
	ItemManager::GetInstance()->Init();
	Field::GetInstance()->Init();
	Wave::GetInstance()->Init();
#pragma region �I�u�W�F�N�g�̏����l�ݒ�


#pragma endregion
	//�w�i�X�v���C�g����
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("white");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	backSprite_->color_.SetColor255(10, 10, 10);

	//�O�i�X�v���C�g����

	Score::Init();
#pragma region �e�X�v���C�g�̐ݒ�
	UI::GetInstance()->SetSize(UIType::Shaft, { 100.0f,100.0f });
	UI::GetInstance()->SetPos(UIType::Shaft, { NWindows::GetInstance()->kWin_width * 0.5f, 500.0f });
	UI::GetInstance()->SetSize(UIType::Lstick, { 100.0f,100.0f });
	UI::GetInstance()->SetPos(UIType::Lstick, { NWindows::GetInstance()->kWin_width * 0.5f, 500.0f });


	UI::GetInstance()->SetPos(UIType::Abutton,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f });
	UI::GetInstance()->SetPos(UIType::AbuttonPush,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f });

	UI::GetInstance()->SetPos(UIType::Clear, { -(float)NWindows::GetInstance()->kWin_width, 100.0f });
	UI::GetInstance()->SetSize(UIType::Clear, { 350.f, 100.f });
	UI::GetInstance()->SetPos(UIType::Faild, { -(float)NWindows::GetInstance()->kWin_width, 100.0f });
	UI::GetInstance()->SetSize(UIType::Faild, { 350.f, 100.f });
#pragma endregion
#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());

	IEmitter3D::SetLightGroup(lightGroup_.get());

	//IPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g����
	Bloom::Init();

	scene = SceneMode::Play;

	stickRotTimer_.Reset();
	slidePos_ = 0.0f;
	slideTimer_.Reset();
	slideTimer_ = 0.1f;
}

void NGameScene::Update()
{
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region �X�v���C�g
	backSprite_->Update();
	Score::Update();
#pragma endregion
	BulletManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();
	ItemManager::GetInstance()->Update();
	Field::GetInstance()->Update();
	Wave::GetInstance()->Update();

	NParticleManager::GetInstance()->Update();

	//���C�g�����̍X�V
	lightGroup_->Update();

	if (scene == SceneMode::Play)	//�v���C���̏���
	{
		Player::GetInstance()->Update();

		//A�{�^���̓v���C���g��Ȃ����猩���Ȃ�
		UI::GetInstance()->SetInvisible(UIType::Abutton, true);
		UI::GetInstance()->SetInvisible(UIType::AbuttonPush, true);

		if (Field::GetInstance()->GetIsStart())
		{
			//�X�^�[�g���X���C�h�^�C�}�[�J�n
			if (slideTimer_.GetStarted() == false)
			{
				slideTimer_.Start();
			}
			slideTimer_.Update();

			slidePos_ = NEasing::InQuad(0.0f, -(float)NWindows::GetInstance()->kWin_width, slideTimer_.GetTimeRate());
			UI::GetInstance()->SetPos(UIType::Shaft,
				{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 500.0f });
		}

		stickRotTimer_.Roop();
		NVec2 stickVec = { sinf(stickRotTimer_.GetTimeRate() * PI2),cosf(stickRotTimer_.GetTimeRate() * PI2) };
		stickVec *= 10.f;
		//Y�̓X�e�B�b�N���Əオ���̒l�Ȃ̂ň���
		UI::GetInstance()->SetPos(UIType::Lstick,
			{ NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos_, 500.0f - stickVec.y });

		if (Field::GetInstance()->GetIsStart())
		{
			//�v���C���[���g�Ɉ��ݍ��܂ꂽ��E��
			if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
			{
				Player::GetInstance()->SetIsAlive(false);
			}
		}

		//�v���C���[������ŁA���S���o���I������玸�s���U���g��
		if (Player::GetInstance()->GetIsAlive() == false &&
			Player::GetInstance()->GetDeadEffectEnd())
		{
			NAudioManager::Destroy("playBGM");
			NAudioManager::Play("faildBGM", true, 0.2f);
			scene = SceneMode::Faild;
			slideTimer_.Reset();
			slideTimer_ = 0.5f;
			Player::GetInstance()->FaildUpdate();	//�����Ńv���C���[�̍��W�ς��Ă����Ȃ��ƃJ�����̍��W�����񂾍��W��ɂȂ����Ⴄ
			Player::GetInstance()->SetIsElapseAnime(false);
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Faild);
		}

		//�S�[��������N���A���U���g��
		if (Field::GetInstance()->GetIsGoal())
		{
			NAudioManager::Destroy("playBGM");
			NAudioManager::Play("clearBGM", true, 0.2f);
			scene = SceneMode::Clear;
			slideTimer_.Reset();
			slideTimer_ = 0.5f;
			Player::GetInstance()->SetIsElapseAnime(false);
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Clear);
		}

		//�����蔻�葍������
		NCollisionManager::GetInstance()->CheckAllCollision();
	}
	else if (scene == SceneMode::Clear)	//�N���A���U���g�̏���
	{
		Player::GetInstance()->ClearUpdate();

		//�X�^�[�g���X���C�h�^�C�}�[�J�n
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
		}
		slideTimer_.Update();
		//�N���A�e�L�X�g�X���C�h
		slidePos_ = NEasing::InOutBack(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
		UI::GetInstance()->SetPos(UIType::Clear,
			{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f });

		//���U���g�X�R�A���ォ�痎���Ă���
		float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer_.GetTimeRate());
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f - Score::GetSize(Score::TexType::Result).x * 2.f, slideP },
			Score::TexType::Result);
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f, slideP - Score::GetSize(Score::TexType::Result).y },
			Score::TexType::Top);

		//A�{�^���_��
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			UI::GetInstance()->SetInvisible(UIType::Abutton, true);
			UI::GetInstance()->SetInvisible(UIType::AbuttonPush, false);
		}
		else
		{
			UI::GetInstance()->SetInvisible(UIType::Abutton, false);
			UI::GetInstance()->SetInvisible(UIType::AbuttonPush, true);
		}

		//�V�[���؂�ւ�
		if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
		{
			NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
		}
	}
	else if (scene == SceneMode::Faild)	//���s���U���g�̏���
	{
		Player::GetInstance()->FaildUpdate();

		//�X�^�[�g���X���C�h�^�C�}�[�J�n
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
		}
		slideTimer_.Update();
		//���s�e�L�X�g�X���C�h
		slidePos_ = NEasing::InQuad(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
		UI::GetInstance()->SetPos(UIType::Faild,
			{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f });

		//���U���g�X�R�A���ォ�痎���Ă���
		float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer_.GetTimeRate());
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f - Score::GetSize(Score::TexType::Result).x * 2.f, slideP },
			Score::TexType::Result);
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f, slideP - Score::GetSize(Score::TexType::Result).y },
			Score::TexType::Top);

		//A�{�^���_��
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			UI::GetInstance()->SetInvisible(UIType::Abutton, true);
			UI::GetInstance()->SetInvisible(UIType::AbuttonPush, false);
		}
		else
		{
			UI::GetInstance()->SetInvisible(UIType::Abutton, false);
			UI::GetInstance()->SetInvisible(UIType::AbuttonPush, true);
		}

		//�V�[���؂�ւ�
		if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
		{
			NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
		}
	}

	//�؂�ւ���ּ���Č���ꂽ��
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NTitleScene>();			//�^�C�g���V�[���ɐ؂�ւ�
		NSceneChange::GetInstance()->SetIsChange(false);	//�؂�ւ�������Ұ
	}

#ifdef _DEBUG
	//�V�[���؂�ւ�(�f�o�b�O�p)
	if (NInput::IsKeyDown(DIK_RETURN) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_X))
	{
		NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
	}

	//���Z�b�g�{�^��(�f�o�b�O�p)
	if (NInput::IsKeyDown(DIK_R) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_START))
	{
		NSceneManager::ChangeScene<NGameScene>();
	}

	//�A�C�e���o���{�^��(�f�o�b�O�p)
	if (NInput::IsKeyDown(DIK_I))
	{
		ItemManager::GetInstance()->Generate(NVec3::zero, BulletType::LineBullet);
	}
#endif
}

void NGameScene::DrawBackSprite()
{
	backSprite_->Draw();
}

void NGameScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
	Wave::GetInstance()->DrawObj();
}

void NGameScene::Draw3D()
{
	BulletManager::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
	ItemManager::GetInstance()->Draw();
	Player::GetInstance()->Draw();
}

void NGameScene::DrawParticle()
{
	NParticleManager::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
	Wave::GetInstance()->DrawSprite();

	UI::GetInstance()->Draw(UIType::Abutton);
	UI::GetInstance()->Draw(UIType::AbuttonPush);
	UI::GetInstance()->Draw(UIType::Clear);
	UI::GetInstance()->Draw(UIType::Faild);
	UI::GetInstance()->Draw(UIType::Shaft);
	UI::GetInstance()->Draw(UIType::Lstick);

	Score::Draw();
}