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
	

	//�I�u�W�F�N�g
	

	//FBX�ǂݍ���
	// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	vertexBuffers.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		vertexBuffers[i]->Init(meshes[i].vertices);
		indexBuffers[i]->Init(meshes[i].indices);
	}

	const wchar_t* modelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";
	std::vector<Mesh> meshes;

	ImportSettings importSetting = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	{
		modelFile,
		meshes,
		false,
		true // �A���V�A�̃��f���́A�e�N�X�`����UV��V�������]���Ă���ۂ��H�̂œǂݍ��ݎ���UV���W���t�]������
	};

	AssimpLoader loader;
	if (!loader.Load(importSetting))
	{
		
	}

	cb.SetHeap();
	cb.SetResource();
	cb.Create();
	cb.Mapping();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	
#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����

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
		NAudioManager::Play("WinSE", false, 0.5f);
	}

	lightGroup->Update();
#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera.CreateMatView();
	NCamera::nowCamera = &camera;

	//timer.Update();
	//if (timer.GetisTimeOut())
	//{
	//	obj[0]->position.x = MathUtil::Random(-1.0f, 1.0f);
	//	timer.Reset();
	//}

	//if (isCol)
	//{
	//	obj[0]->model->material.SetColor(255, 0, 0, 255);
	//	NInput::GetInstance()->Vibration(30000, 1000);
	//}
	//else
	//{
	//	obj[0]->model->material.SetColor(255, 255, 255, 255);
	//	NInput::GetInstance()->Vibration(0, 0);
	//}
	//obj[2]->model->material.SetColor(255, 255, 255, 255);

	//sphere.pos = obj[0]->position;
	//NVector3 vec;
	//plane.distance = obj[2]->position.Dot(plane.normal);

	//for (size_t i = 0; i < maxObj; i++)
	//{
	//	obj[i]->UpdateMatrix();
	//}

	//isCol = NCollision::Sphere2PlaneCol(sphere, plane);
#pragma endregion
}

void NTitleScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g

	//3D�I�u�W�F�N�g

	// ���b�V���̐������C���f�b�N�X���̕`����s����������
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto vbView = vertexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
		auto ibView = indexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());

		//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cb.constBuff->GetGPUVirtualAddress());

		NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

		NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(meshes[i].indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	}

	//�O�i�X�v���C�g
	

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