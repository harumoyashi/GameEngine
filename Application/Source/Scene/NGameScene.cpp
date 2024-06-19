#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NAudioManager.h"
#include "NCollisionManager.h"

SceneMode NGameScene::sScene = SceneMode::Play;

void NGameScene::LoadResources()
{
}

void NGameScene::Init()
{
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion

#pragma region	ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, true, false, false);

	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
#pragma endregion
}

void NGameScene::Update()
{
	if (sScene == SceneMode::Pause)		//ポーズ画面
	{
	}
	else	//ポーズ画面以外の処理
	{
#pragma region カメラ
		NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region スプライト
		
#pragma endregion
		lightGroup_->Update();

		if (sScene == SceneMode::BeforeStart)	//始まる前の処理
		{
		}
		else if (sScene == SceneMode::Play)	//プレイ中の処理
		{
			//当たり判定総当たり
			NCollisionManager::GetInstance()->CheckAllCollision();
		}
		else if (sScene == SceneMode::Clear)	//クリアリザルトの処理
		{
		}
		else if (sScene == SceneMode::Failed)	//失敗リザルトの処理
		{
		}
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
	}
#ifdef _DEBUG
	//シーン切り替え(デバッグ用)
	if (NInput::IsKeyDown(DIK_RETURN) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_X))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}
#endif
}

void NGameScene::DrawBackSprite()
{
}

void NGameScene::DrawBack3D()
{
}

void NGameScene::Draw3D()
{
}

void NGameScene::DrawParticle()
{
}

void NGameScene::DrawForeSprite()
{
}