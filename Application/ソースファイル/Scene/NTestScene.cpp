#include "NDX12.h"
#include "NTestScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "NPostEffect.h"

NTestScene::NTestScene()
{
}

NTestScene::~NTestScene()
{
}

void NTestScene::LoadResources()
{
}

void NTestScene::Init()
{
#pragma region	オーディオ初期化

#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Debug);
#pragma endregion
#pragma region 描画初期化処理
	//オブジェクト
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("boneTest");
	obj_->Init();
#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成

	//前景スプライト生成

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
	//NAssimpModel::SetLightGroup(lightGroup_.get());
}

void NTestScene::Update()
{
#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion

	obj_->Update();

	//ライトたちの更新
	lightGroup_->Update();

	//シーン切り替え
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NTitleScene>();			//タイトルシーンに切り替え
		NSceneChange::GetInstance()->SetIsChange(false);	//切り替えちゃﾀﾞﾒｰ
	}
}

void NTestScene::DrawBackSprite()
{
}

void NTestScene::DrawBack3D()
{
}

void NTestScene::Draw3D()
{
	obj_->Draw();
}

void NTestScene::DrawParticle()
{
}

void NTestScene::DrawForeSprite()
{
}