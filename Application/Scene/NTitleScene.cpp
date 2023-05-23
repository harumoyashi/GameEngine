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
	obj[0]->position = { 0,0,0 };
	obj[1]->position = { 0,-2,0 };
	obj[1]->scale = { 10,0.1f,10 };
	obj[2]->position = { 2,0,0 };
	//�ݒ肵���̂�K�p
	for (int i = 0; i < maxObj; i++)
	{
		obj[i]->UpdateMatrix();
	}

#pragma endregion

	////FBX�ǂݍ���
	//// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	//vertexBuffers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	vertexBuffers[i]->Init(meshes[i].vertices);
	//	indexBuffers[i]->Init(meshes[i].indices);
	//}

	//const wchar_t* modelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";
	//std::vector<Mesh> meshes;

	//ImportSettings importSetting = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	//{
	//	modelFile,
	//	meshes,
	//	false,
	//	true // �A���V�A�̃��f���́A�e�N�X�`����UV��V�������]���Ă���ۂ��H�̂œǂݍ��ݎ���UV���W���t�]������
	//};

	//AssimpLoader loader;
	//if (!loader.Load(importSetting))
	//{
	//	
	//}

	//cbTrans.Init();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	
#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu",{0,0});
	foreSprite[0]->SetPos(900,350);
	foreSprite[0]->SetSize(100,100);
	foreSprite[0]->SetColor(255,255,255,255);

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
		NAudioManager::Play("WinSE", false, 0.5f);
	}

	//���C�g�����̍X�V
	directionalLight->Update();
	pointLight->Update();
	spotLight->Update();
	circleShadow->Update();

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
		//NInput::GetInstance()->Vibration(30000, 1000);
	}
	else
	{
		obj[0]->model->material.SetColor(255, 255, 255, 255);
		//NInput::GetInstance()->Vibration(0, 0);
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

	foreSprite[0]->UpdateMatrix();
#pragma endregion
}

void NTitleScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < obj.size(); i++)
	{
		obj[i]->CommonBeginDraw();
		obj[i]->Draw();
	}

	// ���b�V���̐������C���f�b�N�X���̕`����s����������
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView = vertexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
	//	auto ibView = indexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

	//	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());

	//	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans.constBuff->GetGPUVirtualAddress());

	//	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	//	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

	//	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	//}

	//�O�i�X�v���C�g
	foreSprite[0]->Draw();

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NTitleScene::Reset()
{
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetNDirectionalLight(directionalLight.get());
	NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());
}

void NTitleScene::Finalize()
{
}