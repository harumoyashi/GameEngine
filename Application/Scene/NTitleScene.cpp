#include "NDX12.h"
#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "imgui.h"
#include "NInput.h"
#include "NQuaternion.h"
#include "NMathUtil.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

void NTitleScene::Init()
{
#pragma region	�I�[�f�B�I������
	NAudio::GetInstance()->Init();
	//NAudioManager::Play("WinSE",true,0.2f);
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

	//�I�u�W�F�N�g
	for (int i = 0; i < maxObj; i++)
	{
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0].get());
	obj[1]->SetModel(model[0].get());
	obj[2]->SetModel(model[1].get());

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj[0]->position = { 0,2,0 };
	obj[1]->position = { 2,0,0 };
	obj[2]->position = { 0,0,0 };
	obj[2]->scale = { 10,0.1f,10 };

	//�ݒ肵���̂�K�p
	for (int i = 0; i < maxObj; i++)
	{
		obj[i]->UpdateMatrix();
	}

	sphere.pos = obj[0]->position;
	sphere.radius = obj[0]->scale.x;
	plane.normal = { 0,1,0 };
	plane.distance = obj[2]->position.Length();
#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu");

#pragma endregion
	// ���C�g����
	lightGroup = std::make_unique<NLightGroup>();
	lightGroup = lightGroup->Create();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup.get());

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);

	lightGroup->SetCircleShadowActive(0, true);

	timer.SetMaxTimer(10);
}

void NTitleScene::Update()
{
	ImGui::ShowDemoWindow();

	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(GAMESCENE);
	}

	if (NInput::IsKeyDown(DIK_RETURN))
	{
		NAudioManager::Play("WinSE",false,0.5f);
	}

	lightGroup->Update();
#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera.CreateMatView();
	NCamera::nowCamera = &camera;

	timer.Update();
	if (timer.GetisTimeOut())
	{
		obj[0]->position.x = MathUtil::Random(-1.0f, 1.0f);
		timer.Reset();
	}

	if (isCol)
	{
		obj[0]->model->material.SetColor(255, 0, 0, 255);
		NInput::GetInstance()->Vibration(30000, 1000);
	}
	else
	{
		obj[0]->model->material.SetColor(255, 255, 255, 255);
		NInput::GetInstance()->Vibration(0, 0);
	}
	obj[2]->model->material.SetColor(255, 255, 255, 255);

	sphere.pos = obj[0]->position;
	NVector3 vec;
	plane.distance = obj[2]->position.Dot(plane.normal);

	for (size_t i = 0; i < maxObj; i++)
	{
		obj[i]->UpdateMatrix();
	}

	isCol = NCollision::Sphere2PlaneCol(sphere, plane);
#pragma endregion
}

void NTitleScene::Draw()
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
	if (isCol)
	{
		foreSprite[0]->CommonBeginDraw();
		foreSprite[0]->Draw();
	}

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NTitleScene::Reset()
{
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup.get());

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);

	lightGroup->SetCircleShadowActive(0, true);
}

void NTitleScene::Finalize()
{
}