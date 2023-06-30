#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"

#include "Player.h"

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
	camera_.ProjectiveProjection();
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	Player::GetInstance()->Init();

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

	//�^�C�}�[�X�g�b�v�t���O������
	isMoveUpdate_ = true;
}

void NGameScene::Update()
{
#pragma region �J����
	//�r���[�s��̍Đ���
	//�E�N���b�N������J�������[�h�؂�ւ��
	if (NInput::TriggerMouse(NInput::MouseRight))
	{
		camera_.ChangeIsDebugCamera();
	}
	camera_.Update();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
	//��{������
	isMoveUpdate_ = true;
#pragma region �v���C���[
	Player::GetInstance()->Update();
	//�v���C���[�������ĂȂ��Ƃ��͑��̂��̂������Ȃ�����
	if (Player::GetInstance()->GetMoveVelo().Length() < 0.0f)
	{
		isMoveUpdate_ = false;
	}
#pragma endregion

	//���C�g�����̍X�V
	lightGroup_->Update();

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::ChangeScene<NTitleScene>();
	}
}

void NGameScene::DrawBackSprite()
{
}

void NGameScene::Draw3D()
{
	Player::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
}