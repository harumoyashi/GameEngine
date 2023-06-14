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

NTitleScene::NTitleScene()
{
}

NTitleScene::~NTitleScene()
{
	for (size_t i = 0; i < vertexBuff_ers.size(); i++)
	{
		delete vertexBuff_ers[i];
		delete indexBuff_ers[i];
	}
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
	NCamera::sNowCamera = &camera;
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
	levelData = std::make_unique<LevelData>();
	levelData = LevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/","levelEditor.json");
	//SetObject(levelData.get());
	LevelDataLoader::GetInstance()->SetObject(levelData.get(), levelDataobj);
	for (auto& lo : levelDataobj)
	{
		lo->SetModel(model[0]);
	}


	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj.emplace_back();
		obj[i] = std::make_unique<NObj3d>();
		obj[i]->Init();
	}
	obj[0]->SetModel(model[0]);
	obj[1]->SetModel(model[0]);
	obj[2]->SetModel(model[1]);

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj[0]->position_ = { 0,2,0 };
	obj[1]->position_ = { 2,0,0 };
	obj[2]->position_ = { 0,0,0 };
	obj[2]->scale_ = { 10,0.1f,10 };

	//�ݒ肵���̂�K�p
	for (uint32_t i = 0; i < maxObj; i++)
	{
		obj[i]->Update();
	}

	sphere.pos = obj[0]->position_;
	sphere.radius = obj[0]->scale_.x;
	plane.normal = { 0,1,0 };
	plane.distance = obj[2]->position_.Length();
#pragma endregion

	//FBX�ǂݍ���
	/*if (!loader.Load(importSetting))
	{

	}*/

	//// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	//vertexBuff_ers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = meshes[i].vertices_.size();
	//	auto stride = sizeof(NVertexAssimp);
	//	auto vertices_ = meshes[i].vertices_.data();
	//	auto pVB = new NVertexBuff(vertices_, (unsigned int)size);
	//	/*if (!pVB->IsValid())
	//	{
	//		printf("���_�o�b�t�@�̐����Ɏ��s\n");
	//	}*/

	//	vertexBuff_ers.push_back(pVB);
	//}

	//// ���b�V���̐������C���f�b�N�X�o�b�t�@��p�ӂ���
	//indexBuff_ers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(uint32_t) * meshes[i].indices_.size();
	//	auto indices_ = meshes[i].indices_.data();
	//	auto pIB = new NIndexBuff(indices_, (unsigned int)size);
	//	/*if (!pIB->IsValid())
	//	{
	//		printf("�C���f�b�N�X�o�b�t�@�̐����Ɏ��s\n");
	//		return false;
	//	}*/

	//	indexBuff_ers.push_back(pIB);
	//}

	//// ���f���̃T�C�Y���Ⴄ�̂ŁA�����Ɖf��悤�ɂ��̕ӂ̒l��ς��Ă���
	//auto eyePos = XMVectorSet(0.0f, 120.0, 75.0, 0.0f);
	//auto targetPos = XMVectorSet(0.0f, 120.0, 0.0, 0.0f);
	//auto upward = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//auto fov = XMConvertToRadians(60);

	//assimp�̎l�p�`//
	//NVertexAssimp vertices[4] = {};
	//vertices[0].pos = NVector3(-1.0f, 1.0f, 0.0f);
	//vertices[0].color_ = NColor(1.0f, 0.0f, 0.0f, 1.0f);

	//vertices[1].pos = NVector3(1.0f, 1.0f, 0.0f);
	//vertices[1].color_ = NColor(0.0f, 1.0f, 0.0f, 1.0f);

	//vertices[2].pos = NVector3(1.0f, -1.0f, 0.0f);
	//vertices[2].color_ = NColor(0.0f, 0.0f, 1.0f, 1.0f);

	//vertices[3].pos = NVector3(-1.0f, -1.0f, 0.0f);
	//vertices[3].color_ = NColor(1.0f, 0.0f, 1.0f, 1.0f);

	//uint32_t indices[] = { 0, 1, 2, 0, 2, 3 };

	//Mesh mesh = {}; // ����܂ł̎l�p�`�����邽�߂̃��b�V���\����
	//mesh.vertices_ = std::vector<NVertexAssimp>(std::begin(vertices), std::end(vertices));
	//mesh.indices_ = std::vector<uint32_t>(std::begin(indices), std::end(indices));
	//meshes.clear(); // ������Ɩ��ʊ����邪�A��U�l�p�`�Ŏ��������̂�AssimpLoader�œǂݍ��񂾃��f���̃��b�V������U�N���A
	//meshes.shrink_to_fit(); // ���g���[���ɂ���
	//meshes.push_back(mesh);

	//// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	//vertexBuff_ers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(NVertexAssimp) * meshes[i].vertices_.size();
	//	auto vertices = meshes[i].vertices_.data();
	//	auto pVB = new NVertexBuff(vertices, (unsigned int)size);

	//	vertexBuff_ers.push_back(pVB);
	//}

	//// ���b�V���̐������C���f�b�N�X�o�b�t�@��p�ӂ���
	//indexBuff_ers.reserve(meshes.size());
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto size = sizeof(uint32_t) * meshes[i].indices_.size();
	//	auto indices = meshes[i].indices_.data();
	//	auto pIB = new NIndexBuff(indices, (unsigned int)size);

	//	indexBuff_ers.push_back(pIB);
	//}

	//cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	//cbTrans_->Init();

	//// �萔�o�b�t�@�փf�[�^�]��
	//cbTrans_->constMap_ = nullptr;
	//cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	//NMatrix4 mat;
	//cbTrans_->constMap_->viewproj = NCamera::sNowCamera->GetMatView() * NCamera::sNowCamera->GetMatProjection();
	//cbTrans_->constMap_->world = mat.Identity();
	//cbTrans_->constMap_->cameraPos = NCamera::sNowCamera->GetPos();

	//cbTrans_->Unmap();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	foreSprite[0] = std::make_unique<NSprite>();
	foreSprite[0]->CreateSprite("hamu", { 0,0 });
	foreSprite[0]->SetPos(0, 0);
	foreSprite[0]->SetSize(100, 100);
	foreSprite[0]->color_.SetColor255(255, 255, 255, 255);

#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());

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
	//lightGroup_.get()->sDirLights.SetLightColor({ 0,1.0f,0 });
	lightGroup_->Update();

#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera.CreateMatView();
	NCamera::sNowCamera = &camera;

	if (isCol)
	{
		obj[0]->color_.SetColor255(255, 0, 0, 255);
	}
	else
	{
		obj[0]->color_.SetColor255(255, 255, 255, 255);
	}
	obj[2]->color_.SetColor255(255, 255, 255, 255);

	obj[0]->MoveKey();

	sphere.pos = obj[0]->position_;
	plane.distance = obj[2]->position_.Dot(plane.normal);

	for (auto& o : obj)
	{
		o->Update();
	}

	for (auto& lo : levelDataobj)
	{
		lo->Update();
	}

	isCol = NCollision::Sphere2PlaneCol(sphere, plane);

	foreSprite[0]->Update();
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

	for (size_t i = 0; i < levelDataobj.size(); i++)
	{
		levelDataobj[i]->CommonBeginDraw();
		levelDataobj[i]->Draw();
	}

	////���b�V���̐������C���f�b�N�X���̕`����s����������
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView_ = vertexBuff_ers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
	//	auto ibView = indexBuff_ers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

	//	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState_.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig_.entity_.Get());

	//	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());
	//	//�}�e���A���Ƃ��J���[������Ȃ��Ⴂ���Ȃ����ǈ�U���

	//	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

	//	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices_.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	//}

	// ���b�V���̐������C���f�b�N�X���̕`����s����������
	//for (size_t i = 0; i < meshes.size(); i++)
	//{
	//	auto vbView_ = vertexBuff_ers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@
	//	auto ibView = indexBuff_ers[i]->view; // ���̃��b�V���ɑΉ����钸�_�o�b�t�@

	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig_.entity_.Get());
	//	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState_.Get());
	//	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	//	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());
	//	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView_);
	//	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);

	//	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes[i].indices_.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	//}

	//�O�i�X�v���C�g
	foreSprite[0]->Draw();

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NTitleScene::Reset()
{
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
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
		levelDataobj.emplace_back();
		levelDataobj.back() = std::make_unique<NObj3d>();
		levelDataobj.back()->Init();
		levelDataobj.back()->SetModel(model[0]);

		levelDataobj.back()->position_ = objectData.trans;
		levelDataobj.back()->rotation_ = objectData.rot;
		levelDataobj.back()->scale_ = objectData.scale;
		//obj.back()->SetMatWorld(objectData.matWorld_);
	/*}*/
	}
}
