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
#pragma region	オーディオ初期化
	NAudio::GetInstance()->Init();
	//NAudioManager::Play("RetroBGM",true,0.2f);
#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Debug);
#pragma endregion
#pragma region 描画初期化処理
	//オブジェクト
	//レベルデータからの読み込み
	levelData_ = std::make_unique<LevelData>();
	levelData_ = NLevelDataLoader::GetInstance()->Load("C:/Users/K021G1126/source/repos/GE3/directX_CG/", "levelEditor.json");
	//SetObject(levelData_.get());
	NLevelDataLoader::GetInstance()->SetObject(levelData_.get(), levelDataobj_);
	//レベルデータにあるカメラをデバッグカメラ情報に適用
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

#pragma region オブジェクトの初期値設定
	obj_[0]->position_ = { 0,2,0 };
	obj_[1]->position_ = { 2,0,0 };
	obj_[2]->position_ = { 0,0,0 };
	obj_[2]->scale_ = { 10,0.1f,10 };

	//設定したのを適用
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

#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成
	foreSprite_[0] = std::make_unique<NSprite>();
	foreSprite_[0]->CreateSprite("hamu", { 0,0 });
	foreSprite_[0]->SetPos(0, 0);
	foreSprite_[0]->SetSize(100, 100);
	foreSprite_[0]->color_.SetColor255(255, 255, 255, 255);

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true,false,false,false);
	//lightGroup_->SetSpotLightColor({0,0,1});
	lightGroup_->TransferConstBuffer();
	// 3Dオブジェクトにライトをセット
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

	//ライトたちの更新
	lightGroup_->Update();

#pragma region カメラ
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

	//シーン切り替え
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

	//assimpモデル描画//
	assimpModel_.Draw();
}

void NTitleScene::DrawForeSprite()
{
	foreSprite_[0]->Draw();
}