#include "NDX12.h"
#include "NTitleScene.h"
#include "NSceneManager.h"
#include "NAudioManager.h"
#include "NModelManager.h"
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
}

void NTitleScene::Init()
{
#pragma region	�I�[�f�B�I������
	NAudio::GetInstance()->Init();
	//NAudioManager::Play("RetroBGM",true,0.2f);
#pragma endregion
#pragma region	�J����������
	camera_.SetEye({ 0.0f, 10.0f, 30.0f });
	camera_.SetTarget({ 0.0f, 10.0f, 0.0f });
	camera_.ProjectiveProjection();
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	// ���x���f�[�^����̓ǂݍ���
	levelData_ = std::make_unique<LevelData>();
	levelData_ = NLevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/", "levelEditor.json");
	//SetObject(levelData_.get());
	NLevelDataLoader::GetInstance()->SetObject(levelData_.get(), levelDataobj_);
	for (auto& lo : levelDataobj_)
	{
		lo->SetModel("sphere");
	}

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

	sphere_.pos = obj_[0]->position_;
	sphere_.radius = obj_[0]->scale_.x;
	plane_.normal = { 0,1,0 };
	plane_.distance = obj_[2]->position_.Length();
#pragma endregion

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
	lightGroup_->Init();
	//lightGroup_.get()->sDirLights.SetLightColor({1,0,0});
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
	NAssimpModel::SetLightGroup(lightGroup_.get());

	timer_.SetMaxTimer(10);
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
	lightGroup_->Update();

#pragma region �s��̌v�Z
	//�r���[�s��̍Đ���
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;

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

	sphere_.pos = obj_[0]->position_;
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

	foreSprite_[0]->Update();

	assimpModel_.Update();
#pragma endregion
}

void NTitleScene::Draw()
{
#pragma region �O���t�B�b�N�X�R�}���h
	//�w�i�X�v���C�g

	//3D�I�u�W�F�N�g
	for (size_t i = 0; i < obj_.size(); i++)
	{
		obj_[i]->CommonBeginDraw();
		obj_[i]->Draw();
	}

	for (size_t i = 0; i < levelDataobj_.size(); i++)
	{
		levelDataobj_[i]->CommonBeginDraw();
		levelDataobj_[i]->Draw();
	}

	//assimp���f���`��//
	assimpModel_.Draw();

	//�O�i�X�v���C�g
	foreSprite_[0]->Draw();

	// 4.�`��R�}���h�����܂�
#pragma endregion
}

void NTitleScene::Reset()
{
	lightGroup_->Init();
	//lightGroup_.get()->sDirLights.SetLightColor({1,0,0});
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
	NAssimpModel::SetLightGroup(lightGroup_.get());
}

void NTitleScene::Finalize()
{
}

void NTitleScene::SetObject(LevelData* levelData)
{
	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects)
	{
		/*NModel* model_ = nullptr;
		decltype(levelData->models)::iterator it = levelData->models.find(objectData.filename);
		if (it != levelData->models.end())
		{*/
		/*model_ = &it->second;*/
		//���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		//�z��ɓo�^���Ă�
		levelDataobj_.emplace_back();
		levelDataobj_.back() = std::make_unique<NObj3d>();
		levelDataobj_.back()->Init();
		levelDataobj_.back()->SetModel("sphere");

		levelDataobj_.back()->position_ = objectData.trans;
		levelDataobj_.back()->rotation_ = objectData.rot;
		levelDataobj_.back()->scale_ = objectData.scale;
		//obj_.back()->SetMatWorld(objectData.matWorld_);
	/*}*/
	}
}
