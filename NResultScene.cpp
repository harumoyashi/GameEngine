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
	NCamera::nowCamera = &camera;
#pragma endregion
#pragma region �`�揉��������
	//�}�e���A��(�萔�o�b�t�@)

	//�����̏��

	//���f�����
	for (int i = 0; i < maxModel; i++)
	{
		model[i] = std::make_unique<NModel>();
	}
	model[0]->Create("sphere");
	model[1]->Create("Cube");
	model[2]->Create("busterSword");

	//�I�u�W�F�N�g
	for (int i = 0; i < maxObj; i++)
	{
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0].get());
	obj[1]->SetModel(model[1].get());
	obj[2]->SetModel(model[0].get());
	obj[3]->SetModel(model[2].get());

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj[0]->position = { 0,0,0 };
	obj[1]->position = { 0,-2,0 };
	obj[1]->scale = { 10,0.1f,10 };
	obj[2]->position = { 2,0,0 };
	obj[3]->position = { -2,3,0 };
	//�ݒ肵���̂�K�p
	for (int i = 0; i < maxObj; i++)
	{
		obj[i]->UpdateMatrix();
	}

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����

#pragma endregion
	// ���C�g����
	directionalLight = std::make_unique<NDirectionalLight>();
	directionalLight->Initialize();
	pointLight = std::make_unique<NPointLight>();
	pointLight->Initialize();
	spotLight = std::make_unique<NSpotLight>();
	spotLight->Initialize();
	circleShadow = std::make_unique<NCircleShadow>();
	circleShadow->Initialize();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetNDirectionalLight(directionalLight.get());
	NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());
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
	NCamera::nowCamera = &camera;

	obj[0]->MoveKey();
	obj[3]->MoveKey();

	for (size_t i = 0; i < maxObj; i++)
	{
		obj[i]->UpdateMatrix();
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
	for (int i = 0; i < maxObj; i++)
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
	NObj3d::SetNDirectionalLight(directionalLight.get());
	NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());
}

void NResultScene::Finalize()
{
}