#include "NDX12.h"
#include "NTestScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "Bloom.h"
#include "RadialBlur.h"
#include "GaussianBlur.h"
#include "Player.h"
#include "Field.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

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
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region 描画初期化処理
	Player::GetInstance()->Init();
	Field::GetInstance()->Init();
	for (uint32_t i = 0; i < 8; i++)
	{
		obj_.emplace_back();
		obj_[i] = std::make_unique<NObj3d>();
		obj_[i]->Init();
		obj_[i]->SetModel("cube");

		obj_[i]->scale_ = Player::GetInstance()->GetScale();
	}
	
#pragma region オブジェクトの初期値設定
	obj_[0]->position_ = { -0.1f,-0.2f,-0.1f };
	obj_[1]->position_ = { +0.1f,-0.2f,-0.1f };
	obj_[2]->position_ = { -0.1f,+0.0f,-0.1f };
	obj_[3]->position_ = { +0.1f,+0.0f,-0.1f };
	obj_[4]->position_ = { -0.1f,-0.2f,+0.1f };
	obj_[5]->position_ = { +0.1f,-0.2f,+0.1f };
	obj_[6]->position_ = { -0.1f,+0.0f,+0.1f };
	obj_[7]->position_ = { +0.1f,+0.0f,+0.1f };

#pragma endregion
	//背景スプライト生成
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("logo");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	//backSprite_->color_.SetColor255(50, 50, 50);

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
	//ImGui::ShowDemoWindow();

#ifdef _DEBUG
	ImGui::Begin("PostEffectType");
	static int postEffectNum;
	const char* items[] = { "NoEffect","GaussianBlur","RadianBlur","Bloom" };
	if (ImGui::Combo("PostEffect Choice", &postEffectNum, items, IM_ARRAYSIZE(items)))
	{
		switch (postEffectNum)
		{
		case 0:
			IPostEffect::SetIsActive(false);

			break;

		case 1:
			GaussianBlur::Init();

			break;

		case 2:
			RadialBlur::Init();

			break;

		case 3:
			Bloom::Init();

			break;

		default:
			break;
		}
	}

	ImGui::End();
#endif _DEBUG //ポストエフェクトImGui

#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region スプライト
	backSprite_->Update();
#pragma endregion
	Player::GetInstance()->Update();
	Field::GetInstance()->Update();
	for (auto& obj : obj_)
	{
		obj->Update();
	}

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
	backSprite_->Draw();
}

void NTestScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
}

void NTestScene::Draw3D()
{
	Player::GetInstance()->Draw();
	for (auto& obj : obj_)
	{
		obj->SetBlendMode(BlendMode::None);
		obj->Draw();
		obj->SetBlendMode(BlendMode::None);
	}
}

void NTestScene::DrawParticle()
{
}

void NTestScene::DrawForeSprite()
{
}