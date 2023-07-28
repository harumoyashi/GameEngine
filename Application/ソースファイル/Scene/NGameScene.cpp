#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NAudioManager.h"
#include "NParticleManager.h"
#include "NCollisionManager.h"
#include "NPostEffect.h"

#include "Player.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "Field.h"
#include "Wave.h"

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
	BulletManager::GetInstance()->Init();
	EnemyManager::GetInstance()->Init();
	Field::GetInstance()->Init();
	Wave::GetInstance()->Init();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�


#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i] = std::make_unique<NSprite>();
	}
#pragma region �e�X�v���C�g�̐ݒ�
	foreSprite_[(uint32_t)FSpriteType::Shaft]->CreateSprite("shaft");
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 500.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->CreateSprite("stick");
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 500.0f);

	for (uint32_t i = 0; i < 2; i++)
	{
		foreSprite_[(uint32_t)FSpriteType::Abutton + i] = std::make_unique<NSprite>();
		foreSprite_[(uint32_t)FSpriteType::Abutton + i]->CreateClipSprite("Abutton", { i * 192.f,0 }, { 192.f,192.f });
		foreSprite_[(uint32_t)FSpriteType::Abutton + i]->SetPos(
			(float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f);
	}

	foreSprite_[(uint32_t)FSpriteType::Clear]->CreateSprite("clear");
	foreSprite_[(uint32_t)FSpriteType::Clear]->SetPos(-(float)NWindows::GetInstance()->kWin_width, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Faild]->CreateSprite("faild");
	foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(-(float)NWindows::GetInstance()->kWin_width, 100.0f);
#pragma endregion
#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());

	IEmitter3D::SetLightGroup(lightGroup_.get());

	NPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g����

	scene = SceneMode::Play;

	slidePos = 0.0f;
	slideTimer.Reset();
	slideTimer = 0.1f;
}

void NGameScene::Update()
{
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region �X�v���C�g
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Update();
	}

#pragma endregion
	BulletManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();
	Field::GetInstance()->Update();

	NParticleManager::GetInstance()->Update();

	//���C�g�����̍X�V
	lightGroup_->Update();

	if (scene == SceneMode::Play)	//�v���C���̏���
	{
		Player::GetInstance()->Update();
		Wave::GetInstance()->Update();
		//���S�p�[�e�B�N���o��{�^��
		if (NInput::IsKeyDown(DIK_0))
		{
			Player::GetInstance()->DeadParticle();
		}

		//A�{�^���̓v���C���g��Ȃ����猩���Ȃ�
		foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
		foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;

		if (Field::GetInstance()->GetIsStart())
		{
			//�X�^�[�g���X���C�h�^�C�}�[�J�n
			if (slideTimer.GetStarted() == false)
			{
				slideTimer.Start();
			}
			slideTimer.Update();

			slidePos = NEasing::InQuad(0.0f, -(float)NWindows::GetInstance()->kWin_width, slideTimer.GetTimeRate());
			foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(
				NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 500.0f);
		}

		NVector2 stickVec;
		stickVec = NInput::GetInstance()->GetStick() * 8.0f;
		//Y�̓X�e�B�b�N���Əオ���̒l�Ȃ̂ň���
		foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(
			NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos, 500.0f - stickVec.y);

		//�v���C���[���g�Ɉ��ݍ��܂ꂽ��E��
		if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
		{
			Player::GetInstance()->DeadParticle();
			Player::GetInstance()->SetIsAlive(false);
		}

		//�v���C���[������ŁA���S���o���I������玸�s���U���g��
		if (Player::GetInstance()->GetIsAlive() == false &&
			Player::GetInstance()->GetDeadEffectEnd())
		{
			scene = SceneMode::Faild;
			slideTimer.Reset();
			slideTimer = 0.5f;
			Player::GetInstance()->FaildUpdate();	//�����Ńv���C���[�̍��W�ς��Ă����Ȃ��ƃJ�����̍��W�����񂾍��W��ɂȂ����Ⴄ
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Faild);
		}

		if (Field::GetInstance()->GetIsGoal())
		{
			scene = SceneMode::Clear;
			slideTimer.Reset();
			slideTimer = 0.5f;
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Clear);
		}

		//�����蔻�葍������
		NCollisionManager::GetInstance()->CheckAllCollision();
	}
	else if (scene == SceneMode::Clear)	//�N���A���U���g�̏���
	{
		Player::GetInstance()->ClearUpdate();

		//�X�^�[�g���X���C�h�^�C�}�[�J�n
		if (slideTimer.GetStarted() == false)
		{
			slideTimer.Start();
		}
		slideTimer.Update();
		//�N���A�e�L�X�g�X���C�h
		slidePos = NEasing::InOutBack(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Clear]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 100.0f);

		//A�{�^���_��
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = false;
		}
		else
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = false;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;
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
		if (slideTimer.GetStarted() == false)
		{
			slideTimer.Start();
		}
		slideTimer.Update();
		//���s�e�L�X�g�X���C�h
		slidePos = NEasing::InQuad(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 100.0f);

		//A�{�^���_��
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = false;
		}
		else
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = false;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;
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
}

void NGameScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
	Wave::GetInstance()->Draw();
}

void NGameScene::DrawBackSprite()
{
}

void NGameScene::Draw3D()
{
	BulletManager::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
	Player::GetInstance()->Draw();
}

void NGameScene::DrawParticle()
{
	NParticleManager::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Draw();
	}
}