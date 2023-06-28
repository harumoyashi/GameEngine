#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"

#include "Player.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
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
}

void NGameScene::Update()
{
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
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
	Player::GetInstance()->Update();

	//���C�g�����̍X�V
	lightGroup_->Update();
}

void NGameScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g
	NSprite::CommonBeginDraw();

	//3D�I�u�W�F�N�g
	NObj3d::CommonBeginDraw();
	Player::GetInstance()->Draw();
	
	//�O�i�X�v���C�g
	NSprite::CommonBeginDraw();

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NGameScene::Reset()
{
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NGameScene::Finalize()
{
}