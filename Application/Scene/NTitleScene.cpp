#include "NDX12.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "NQuaternion.h"
#include "NMathUtil.h"
#include "NCameraManager.h"

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
}

void NTitleScene::LoadResources()
{
}

void NTitleScene::Init()
{
#pragma region	�I�[�f�B�I������
	NAudio::GetInstance()->Init();
	//NAudioManager::Play("RetroBGM",true,0.2f);
#pragma endregion
#pragma region	�J����������
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Debug);
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	//���x���f�[�^����̓ǂݍ���
	levelData_ = std::make_unique<LevelData>();
	levelData_ = NLevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/", "levelEditor.json");
	//SetObject(levelData_.get());
	NLevelDataLoader::GetInstance()->SetObject(levelData_.get(), levelDataobj_);
	//���x���f�[�^�ɂ���J�������f�o�b�O�J�������ɓK�p
	NCamera camera = NLevelDataLoader::GetInstance()->SetCamera(levelData_.get());
	NCameraManager::GetInstance()->SetDebugCamera(camera);
	
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_.emplace_back();
		obj_[i] = std::make_unique<NObj3d>();
		obj_[i]->Init();
	}
	obj_[0]->SetModel("sphere");
	obj_[1]->SetModel("sphere");
	obj_[2]->SetModel("cube");

#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj_[0]->position_ = { 0,2,0 };
	obj_[1]->position_ = { 2,0,0 };
	obj_[2]->position_ = { 0,0,0 };
	obj_[2]->scale_ = { 10,0.1f,10 };

	//�ݒ肵���̂�K�p
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Update();
	}

	sphere_.centerPos = obj_[0]->position_;
	sphere_.radius = obj_[0]->scale_.x;
	plane_.normal = { 0,1,0 };
	plane_.distance = obj_[2]->position_.Length();
#pragma endregion

	//assimpModel_.Load(L"Resources/FBX/Alicia_solid_Unity.FBX");
	//assimpModel_.Load(L"Resources/Tripping.fbx");
	assimpModel_.Load(L"Resources/Cat_fixed.fbx");
	assimpModel_.Init();

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	foreSprite_[0] = std::make_unique<NSprite>();
	foreSprite_[0]->CreateSprite("hamu", { 0,0 });
	foreSprite_[0]->SetPos(0, 0);
	foreSprite_[0]->SetSize(100, 100);
	foreSprite_[0]->color_.SetColor255(255, 255, 255, 255);

#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true,false,false,false);
	//lightGroup_->SetSpotLightColor({0,0,1});
	lightGroup_->TransferConstBuffer();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
	NAssimpModel::SetLightGroup(lightGroup_.get());

	timer_.SetMaxTimer(10);
}

void NTitleScene::Update()
{
	if (NInput::IsKeyDown(DIK_RETURN))
	{
		NAudioManager::Play("WinSE", false, 0.5f);
	}

	//���C�g�����̍X�V
	lightGroup_->Update();

#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
	if (isCol_)
	{
		obj_[0]->color_.SetColor255(255, 0, 0, 255);
	}
	else
	{
		obj_[0]->color_.SetColor255(255, 255, 255, 255);
	}
	obj_[2]->color_.SetColor255(255, 255, 255, 255);

	obj_[0]->MoveKey();

	sphere_.centerPos = obj_[0]->position_;
	plane_.distance = obj_[2]->position_.Dot(plane_.normal);

	for (auto& o : obj_)
	{
		o->Update();
	}

	for (auto& lo : levelDataobj_)
	{
		lo->Update();
	}

	isCol_ = NCollision::Sphere2PlaneCol(sphere_, plane_);

	//foreSprite_[0]->Update();

	assimpModel_.Update();

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::ChangeScene<NGameScene>();
	}

}

void NTitleScene::DrawBackSprite()
{
}

void NTitleScene::Draw3D()
{
	for (size_t i = 0; i < obj_.size(); i++)
	{
		obj_[i]->Draw();
	}

	for (size_t i = 0; i < levelDataobj_.size(); i++)
	{
		levelDataobj_[i]->Draw();
	}

	//assimp���f���`��//
	assimpModel_.Draw();
}

void NTitleScene::DrawForeSprite()
{
	foreSprite_[0]->Draw();
}