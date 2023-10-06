#include "NDX12.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "NMathUtil.h"
#include "NCameraManager.h"
#include "IPostEffect.h"
#include "Bloom.h"

#include "Player.h"
#include "Field.h"

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
	NAudioManager::GetInstance()->Play("titleBGM", true, 0.2f);
#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Title);
#pragma endregion
#pragma region 描画初期化処理

#pragma region オブジェクトの初期値設定

#pragma endregion
	Player::GetInstance()->Init();

#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("white");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	backSprite_->color_.SetColor255(10, 10, 10);

	//前景スプライト生成
	titleLogo_ = std::make_unique<NSprite>();
	titleLogo_->CreateSprite("logo");
	titleLogo_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	titleLogo_->SetSize(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	titleLogo_->color_.SetColor255(255, 255, 255, 255);

	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i] = std::make_unique<NSprite>();
		aButton_[i]->CreateClipSprite("Abutton", { i * 192.f,0 }, { 192.f,192.f });
		aButton_[i]->SetPos(
			(float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f);
	}

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, false, false, false);
	lightGroup_->TransferConstBuffer();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());

	//IPostEffect::SetIsActive(false);	//ポストエフェクト消す
	Bloom::Init();
}

void NTitleScene::Update()
{
	//ライトたちの更新
	lightGroup_->Update();

#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
	backSprite_->Update();
	titleLogo_->Update();
	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i]->Update();
	}

	flashingTimer_.Roop();
	if (flashingTimer_.GetTimeRate() > 0.7f)
	{
		aButton_[0]->isInvisible_ = true;
		aButton_[1]->isInvisible_ = false;
	}
	else
	{
		aButton_[0]->isInvisible_ = false;
		aButton_[1]->isInvisible_ = true;
	}

	Player::GetInstance()->SetIsMove(false);
	Player::GetInstance()->Update();

	//シーン切り替え
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NAudioManager::GetInstance()->Destroy("titleBGM");
		NSceneManager::ChangeScene<NGameScene>();			//タイトルシーンに切り替え
		NSceneChange::GetInstance()->SetIsChange(false);	//切り替えちゃﾀﾞﾒｰ
	}
}

void NTitleScene::DrawBackSprite()
{
	backSprite_->Draw();
}

void NTitleScene::DrawBack3D()
{
}

void NTitleScene::Draw3D()
{
	Player::GetInstance()->Draw();
}

void NTitleScene::DrawParticle()
{
}

void NTitleScene::DrawForeSprite()
{
	titleLogo_->Draw();
	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i]->Draw();
	}
}