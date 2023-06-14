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
	audio = NAudio::GetInstance();
#pragma endregion
#pragma region	�J����������
	camera.ProjectiveProjection();
	camera.CreateMatView();
	NCamera::sCurrentCamera = &camera;
#pragma endregion
#pragma region �`�揉��������
	//�}�e���A��(�萔�o�b�t�@)

	//�����̏��

	//���f�����
	for (size_t i = 0; i < maxModel; i++)
	{
		model.emplace_back();
	}
	model[0].Create("sphere");
	model[1].Create("Cube");
	model[2].Create("busterSword");

	//�I�u�W�F�N�g
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[1]);
	obj[2]->SetModel(model[0]);
	obj[3]->SetModel(model[2]);

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj[0]->position_ = { 0,0,0 };
	obj[1]->position_ = { 0,-2,0 };
	obj[1]->scale_ = { 10,0.1f,10 };
	obj[2]->position_ = { 2,0,0 };
	obj[3]->position_ = { -2,3,0 };
	//�ݒ肵���̂�K�p
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����

#pragma endregion
	// ���C�g����
	directionalLight = std::make_unique<NDirectionalLight>();
	directionalLight->Init();
	pointLight = std::make_unique<NPointLight>();
	pointLight->Init();
	spotLight = std::make_unique<NSpotLight>();
	spotLight->Init();
	circleShadow = std::make_unique<NCircleShadow>();
	circleShadow->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	
}

void NResultScene::Update()
{
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera.CreateMatView();
	NCamera::sCurrentCamera = &camera;

	obj[0]->MoveKey();
	obj[3]->MoveKey();

	for (size_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}
#pragma endregion

	//���C�g�����̍X�V
	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();
	circleShadow->Update();
}

void NResultScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g

	//3D�I�u�W�F�N�g
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj[i]->CommonBeginDraw();
		obj[i]->Draw();
	}

	//�O�i�X�v���C�g

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NResultScene::Reset()
{
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	
}

void NResultScene::Finalize()
{
}