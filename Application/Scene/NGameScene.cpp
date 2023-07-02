#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"
#include "NCameraManager.h"

#include "Player.h"
#include "BulletManager.h"
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
	audio_ = NAudio::GetInstance();
#pragma endregion
#pragma region	�J����������
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	Player::GetInstance()->Init();
	BulletManager::GetInstance()->Init();
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
}

void NGameScene::Update()
{
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region �v���C���[
	Player::GetInstance()->Update();
	
#pragma endregion
	BulletManager::GetInstance()->Update();
	Field::GetInstance()->Update();
	Wave::GetInstance()->Update();

	if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
	{
		Player::GetInstance()->SetIsAlive(false);
	}

	//���C�g�����̍X�V
	lightGroup_->Update();

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_X))
	{
		NSceneManager::ChangeScene<NTitleScene>();
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
	Player::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
}