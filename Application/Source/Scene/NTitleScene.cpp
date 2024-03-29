#include "NDX12.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NParticleManager.h"
#include "NInput.h"
#include "NMathUtil.h"
#include "NCameraManager.h"
#include "IPostEffect.h"
#include "Bloom.h"
#include "UI.h"

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
	NParticleManager::GetInstance()->Init();
	Player::GetInstance()->Init();
	Field::GetInstance()->Init();
	Field::GetInstance()->SetIsGoal(true);
#pragma endregion

#pragma region オブジェクトの初期値設定

#pragma endregion
	//背景スプライト生成
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("white");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	backSprite_->color_.SetColor255(1, 1, 1);

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

	UIManager::GetInstance()->SetSize(UIType::Abutton,
		{ 120.f, 120.f });
	UIManager::GetInstance()->SetPos(UIType::Abutton,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 620.f });
	UIManager::GetInstance()->SetSize(UIType::AbuttonPush,
		{ 120.f, 120.f });
	UIManager::GetInstance()->SetPos(UIType::AbuttonPush,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 620.f });

#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, false, false, false);

	for (auto& pointLight : lightGroup_->sPointLights)
	{
		pointLight->SetActive(false);
	}
	lightGroup_->sPointLights[0]->SetActive(true);
	lightGroup_->sPointLights[0]->SetLightAtten({ 0.05f,0.05f,0.05f });
	lightGroup_->sPointLights[0]->SetLightColor(Player::GetInstance()->GetColor().GetRGB());

	lightGroup_->Update();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
	IEmitter3D::SetLightGroup(lightGroup_.get());

	//IPostEffect::SetIsActive(false);	//ポストエフェクト消す
	Bloom::Init();

	//その他
	logoMoveTimer_.Reset();
}

void NTitleScene::Update()
{
	//ライトたちの更新
	lightGroup_->sPointLights[0]->SetLightPos(Player::GetInstance()->GetHeadPos() + NVec3(0, 0.5f, 0));
	lightGroup_->Update();

#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
	backSprite_->Update();

	logoMoveTimer_.RoopReverse();
	logoMoveTimer_.Update();

	logoPosY_ = 
		(float)NWindows::GetInstance()->kWin_height * 0.5f +
		NEasing::InBack(logoMoveTimer_.GetTimeRate()) * 15.f;

	titleLogo_->position_.y = logoPosY_;
	titleLogo_->Update();

	Field::GetInstance()->Update();
	Field::GetInstance()->StopSE();


	flashingTimer_.Roop();
	if (flashingTimer_.GetTimeRate() > 0.7f)
	{
		UIManager::GetInstance()->SetInvisible(UIType::Abutton,false);
		UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush,true);
	}
	else
	{
		UIManager::GetInstance()->SetInvisible(UIType::Abutton, true);
		UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, false);
	}

	//タイトル用の更新処理する
	Player::GetInstance()->TitleUpdate();

	NParticleManager::GetInstance()->Update();

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
	Field::GetInstance()->Draw();
}

void NTitleScene::Draw3D()
{
	Player::GetInstance()->Draw();
}

void NTitleScene::DrawParticle()
{
	NParticleManager::GetInstance()->Draw();
}

void NTitleScene::DrawForeSprite()
{
	titleLogo_->Draw();
	UIManager::GetInstance()->Draw(UIType::Abutton);
	UIManager::GetInstance()->Draw(UIType::AbuttonPush);
}