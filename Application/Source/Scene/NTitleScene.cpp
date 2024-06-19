#include "NDX12.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NInput.h"
#include "NCameraManager.h"

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
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Title);
#pragma endregion
#pragma region 描画初期化処理

#pragma region オブジェクトの初期値設定
	
#pragma endregion

#pragma region オブジェクトの初期値設定

#pragma endregion
	
#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, false, false, false);

	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NTitleScene::Update()
{
	//ライトたちの更新
	lightGroup_->Update();

#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
	//シーン切り替え
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
	}
}

void NTitleScene::DrawBackSprite()
{
}

void NTitleScene::DrawBack3D()
{
}

void NTitleScene::Draw3D()
{
}

void NTitleScene::DrawParticle()
{
}

void NTitleScene::DrawForeSprite()
{
}