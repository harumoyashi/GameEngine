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

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

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

#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());

	IEmitter3D::SetLightGroup(lightGroup_.get());

	NPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g����

	scene = SceneMode::Play;
}

void NGameScene::Update()
{
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
	Player::GetInstance()->Update();
	BulletManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();
	Field::GetInstance()->Update();

	NParticleManager::GetInstance()->Update();

	//���C�g�����̍X�V
	lightGroup_->Update();

	NCollisionManager::GetInstance()->CheckAllCollision();

	if (scene == SceneMode::Play)	//�v���C���̏���
	{
		Wave::GetInstance()->Update();
		//���S�p�[�e�B�N���o��{�^��
		if (NInput::IsKeyDown(DIK_0))
		{
			Player::GetInstance()->DeadParticle();
		}

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
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Result);
		}
	}
	else if (scene == SceneMode::Clear)	//�N���A���U���g�̏���
	{

	}
	else if (scene == SceneMode::Faild)	//���s���U���g�̏���
	{

	}

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
	}

	//�؂�ւ���ּ���Č���ꂽ��
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NTitleScene>();			//�^�C�g���V�[���ɐ؂�ւ�
		NSceneChange::GetInstance()->SetIsChange(false);	//�؂�ւ�������Ұ
	}

	//���Z�b�g�{�^��
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
}