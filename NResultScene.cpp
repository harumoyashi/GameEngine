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
	lightGroup = std::make_unique<NLightGroup>();
	lightGroup = lightGroup->Create();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup.get());

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetSpotLightActive(1, false);
	lightGroup->SetSpotLightActive(2, false);

	lightGroup->SetCircleShadowActive(0, false);
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
	NObj3d::SetLightGroup(lightGroup.get());

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetSpotLightActive(1, false);
	lightGroup->SetSpotLightActive(2, false);

	lightGroup->SetCircleShadowActive(0, false);
}

void NResultScene::Finalize()
{
}