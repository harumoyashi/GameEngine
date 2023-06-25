#include "NDX12.h"
#include "NResultScene.h"
#include "NSceneManager.h"

NResultScene* NResultScene::GetInstance()
{
	static NResultScene instance;
	return &instance;
}

void NResultScene::Init()
{
#pragma region	�I�[�f�B�I������
	
#pragma endregion
#pragma region	�J����������
	camera_.ProjectiveProjection();
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_.emplace_back();
		obj_[i] = std::make_unique<NObj3d>();
		obj_[i]->Init();
	}
	obj_[0]->SetModel("sphere");
	obj_[1]->SetModel("cube");
	obj_[2]->SetModel("sphere");
	obj_[3]->SetModel("busterSword");

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj_[0]->position_ = { 0,0,0 };
	obj_[1]->position_ = { 0,-2,0 };
	obj_[1]->scale_ = { 10,0.1f,10 };
	obj_[2]->position_ = { 2,0,0 };
	obj_[3]->position_ = { -2,3,0 };
	//�ݒ肵���̂�K�p
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Update();
	}

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

void NResultScene::Update()
{
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;

	obj_[0]->MoveKey();
	obj_[3]->MoveKey();

	for (size_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Update();
	}
#pragma endregion

	//���C�g�����̍X�V
	lightGroup_->Update();
}

void NResultScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g

	//3D�I�u�W�F�N�g
	NObj3d::CommonBeginDraw();
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Draw();
	}

	//�O�i�X�v���C�g

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NResultScene::Reset()
{
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NResultScene::Finalize()
{
}