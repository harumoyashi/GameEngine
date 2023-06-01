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

NTitleScene::NTitleScene() :cbTrans(new NConstBuff<ConstBuffDataTransform>)
{
}

NTitleScene::~NTitleScene()
{
	delete cbTrans;
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

	//���f�����
	for (size_t i = 0; i < maxModel; i++)
	{
		model.emplace_back();
	}
	model[0].Create("sphere");
	model[1].Create("Cube");

	//�I�u�W�F�N�g
	// ���x���f�[�^����̓ǂݍ���
	//levelData = LevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/","levelEditor.json");
	//SetObject(levelData);

	for (int i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[0]);
	obj[2]->SetModel(model[1]);

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj[0]->position = { 0,2,0 };
	obj[1]->position = { 2,0,0 };
	obj[2]->position = { 0,0,0 };
	obj[2]->scale = { 10,0.1f,10 };

	//�ݒ肵���̂�K�p
	for (int i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

	sphere.pos = obj[0]->position;
	sphere.radius = obj[0]->scale.x;
	plane.normal = { 0,1,0 };
	plane.distance = obj[2]->position.Length();
#pragma endregion

	////FBX�ǂݍ���
	//const wchar_t* modelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";

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

	//}

	//// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	//vertexBuffers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = meshes[i].vertices.size();
	//	auto stride = sizeof(NVertexAssimp);
	//	auto vertices = meshes[i].vertices.data();
	//	auto pVB = new NVertexBuff(vertices, (unsigned int)size);
	//	/*if (!pVB->IsValid())
	//	{
	//		printf("���_�o�b�t�@�̐����Ɏ��s\n");
	//	}*/

	//	vertexBuffers.push_back(pVB);
	//}

	//// ���b�V���̐������C���f�b�N�X�o�b�t�@��p�ӂ���
	//indexBuffers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(uint32_t) * meshes[i].indices.size();
	//	auto indices = meshes[i].indices.data();
	//	auto pIB = new NIndexBuff(indices, (unsigned int)size);
	//	/*if (!pIB->IsValid())
	//	{
	//		printf("�C���f�b�N�X�o�b�t�@�̐����Ɏ��s\n");
	//		return false;
	//	}*/

	//	indexBuffers.push_back(pIB);
	//}

	//// ���f���̃T�C�Y���Ⴄ�̂ŁA�����Ɖf��悤�ɂ��̕ӂ̒l��ς��Ă���
	//auto eyePos = XMVectorSet(0.0f, 120.0, 75.0, 0.0f);
	//auto targetPos = XMVectorSet(0.0f, 120.0, 0.0, 0.0f);
	//auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//auto fov = XMConvertToRadians(60);

	//cbTrans->Init();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu", { 0,0 });
	foreSprite[0]->SetPos(0, 0);
	foreSprite[0]->SetSize(100, 100);
	foreSprite[0]->SetColor(255, 255, 255, 255);

#pragma endregion
	// ���C�g����
	directionalLight = std::make_unique<NDirectionalLight>();
	directionalLight->Initialize();
	/*pointLight = std::make_unique<NPointLight>();
	pointLight->Initialize();
	spotLight = std::make_unique<NSpotLight>();
	spotLight->Initialize();
	circleShadow = std::make_unique<NCircleShadow>();
	circleShadow->Initialize();*/
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetNDirectionalLight(directionalLight.get());
	/*NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());*/

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
	/*pointLight->Update();
	spotLight->Update();
	circleShadow->Update();*/

#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera.CreateMatView();
	NCamera::nowCamera = &camera;

	/*if (isCol)
	{
		obj[0]->model->material.SetColor(255, 0, 0, 255);
	}
	else
	{
		obj[0]->model->material.SetColor(255, 255, 255, 255);
	}
	obj[2]->model->material.SetColor(255, 255, 255, 255);*/

	//obj[0]->MoveKey();

	/*sphere.pos = obj[0]->position;
	plane.distance = obj[2]->position.Dot(plane.normal);*/

	for (auto& o : obj)
	{
		o->Update();
	}

	/*isCol = NCollision::Sphere2PlaneCol(sphere, plane);*/

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

	////���b�V���̐������C���f�b�N�X���̕`����s����������
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView = vertexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
	//	auto ibView = indexBuffers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

	//	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());

	//	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans->constBuff->GetGPUVirtualAddress());

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
	directionalLight->Initialize();
	/*pointLight->Initialize();
	spotLight->Initialize();
	circleShadow->Initialize();*/
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetNDirectionalLight(directionalLight.get());
	/*NObj3d::SetNPointLight(pointLight.get());
	NObj3d::SetNSpotLight(spotLight.get());
	NObj3d::SetNCircleShadow(circleShadow.get());*/
}

void NTitleScene::Finalize()
{
}

void NTitleScene::SetObject(LevelData* levelData)
{
	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects)
	{
		/*NModel* model = nullptr;
		decltype(levelData->models)::iterator it = levelData->models.find(objectData.filename);
		if (it != levelData->models.end())
		{*/
		/*model = &it->second;*/
		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		//�z��ɓo�^���Ă�
		obj.emplace_back();
		obj.back() = std::make_unique<NObj3d>();
		obj.back()->Init();
		obj.back()->SetModel(model[0]);

		obj.back()->position = objectData.trans;
		obj.back()->rotation = objectData.rot;
		obj.back()->scale = objectData.scale;
		//obj.back()->SetMatWorld(objectData.matWorld);
	/*}*/
	}
}
