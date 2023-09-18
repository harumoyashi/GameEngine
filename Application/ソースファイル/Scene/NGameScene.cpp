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
#include "ItemManager.h"
#include "Field.h"
#include "Wave.h"
#include "Score.h"

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
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i] = std::make_unique<NSprite>();
	}

	Score::Init();
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
	foreSprite_[(uint32_t)FSpriteType::Clear]->SetSize(350.f, 100.f);
	foreSprite_[(uint32_t)FSpriteType::Faild]->CreateSprite("faild");
	foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(-(float)NWindows::GetInstance()->kWin_width, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Faild]->SetSize(350.f, 100.f);
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

	slidePos_ = 0.0f;
	slideTimer_.Reset();
	slideTimer_ = 0.1f;

	//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɓo�^
	NParticleManager::GetInstance()->AddEmitter(&clearParticle_, "gameClear");
	clearParticle_.SetIsRotation(true);
	clearParticleTimer_.Reset();
}

void NGameScene::Update()
{
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region �X�v���C�g
	backSprite_->Update();
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Update();
	}
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
			if (slideTimer_.GetStarted() == false)
			{
				slideTimer_.Start();
			}
			slideTimer_.Update();

			slidePos_ = NEasing::InQuad(0.0f, -(float)NWindows::GetInstance()->kWin_width, slideTimer_.GetTimeRate());
			foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(
				NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 500.0f);
		}

		NVec2 stickVec;
		stickVec = NInput::GetInstance()->GetStick() * 8.0f;
		//Y�̓X�e�B�b�N���Əオ���̒l�Ȃ̂ň���
		foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(
			NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos_, 500.0f - stickVec.y);

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

		//�N���A���p�[�e�B�N���p�^�C�}�[�J�n
		clearParticleTimer_.Roop();
		clearParticleTimer_.Update();

		//�N���A���ɃN���b�J�[�݂����ȃp�[�e�B�N�����ʂ蓹�ɏo����
		if (clearParticleTimer_.GetTimeRate() <= 0.0f)
		{
			for (uint32_t i = 0; i < 7; i++)
			{
				clearParticle_.SetPos(Player::GetInstance()->GetPos() + NVec3(7.f, 0.f, (float)i * -7.f));
				clearParticle_.Add(
					10, 1.5f, NColor::kWhite, 0.1f, 0.8f,
					{ -0.3f,0.1f,-0.3f }, { 0.3f,0.5f,0.3f },
					NVec3::zero, -NVec3::one, NVec3::one);
			}
		}

		//�X�^�[�g���X���C�h�^�C�}�[�J�n
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
		}
		slideTimer_.Update();
		//�N���A�e�L�X�g�X���C�h
		slidePos_ = NEasing::InOutBack(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Clear]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f);

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
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
		}
		slideTimer_.Update();
		//���s�e�L�X�g�X���C�h
		slidePos_ = NEasing::InQuad(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f);

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

	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Draw();
	}

	Score::Draw();
}