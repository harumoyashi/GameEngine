#include "NDX12.h"
#include "NResultScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"
#include "NCameraManager.h"

NResultScene* NResultScene::GetInstance()
{
	static NResultScene instance;
	return &instance;
}

void NResultScene::Init()
{
#pragma region	オーディオ初期化
	
#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region 描画初期化処理
	//オブジェクト
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_.emplace_back();
		obj_[i] = std::make_unique<NObj3d>();
		obj_[i]->Init();
	}
	obj_[0]->SetModel("sphere");
	obj_[1]->SetModel("cube");
	obj_[2]->SetModel("sphere");
	obj_[3]->SetModel("busterSword");

#pragma region オブジェクトの初期値設定
	obj_[0]->position_ = { 0,0,0 };
	obj_[1]->position_ = { 0,-2,0 };
	obj_[1]->scale_ = { 10,0.1f,10 };
	obj_[2]->position_ = { 2,0,0 };
	obj_[3]->position_ = { -2,3,0 };
	//設定したのを適用
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Update();
	}

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NResultScene::Update()
{
#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion

	obj_[0]->MoveKey();
	obj_[3]->MoveKey();

	for (size_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Update();
	}

	//ライトたちの更新
	lightGroup_->Update();

	//シーン切り替え
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::ChangeScene<NTitleScene>();
	}
}

void NResultScene::Draw()
{
#pragma region グラフィックスコマンド
	//背景スプライト
	NSprite::CommonBeginDraw();

	//3Dオブジェクト
	NObj3d::CommonBeginDraw();
	for (uint32_t i = 0; i < kMaxObj; i++)
	{
		obj_[i]->Draw();
	}

	//前景スプライト
	NSprite::CommonBeginDraw();

	// 4.描画コマンドここまで
#pragma endregion
}

void NResultScene::Reset()
{
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NResultScene::Finalize()
{
}