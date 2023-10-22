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

	for (uint32_t i = 0; i < kMaxFragment_; i++)
	{
		fragment_[i].obj = std::make_unique<NObj3d>();
		fragment_[i].obj->Init();
		fragment_[i].obj->SetModel("cube");

		float dist = Player::GetInstance()->GetScale().x * 2.f;
		fragment_[i].oriPos =
		{ (float)(i % fragmentNum_) * dist,0.f,(float)(i / fragmentNum_) * dist };

		fragment_[i].obj->scale_ = Player::GetInstance()->GetScale();
		fragment_[i].obj->position_ = fragment_[i].oriPos;
		if (i % 2 == 1)
		{
			fragment_[i].obj->color_ = NColor::kLightblue;
		}

		fragment_[i].floatingTimer = MathUtil::Randomf(1.f, 3.f);	//1~3秒の間で設定
	}

#pragma region オブジェクトの初期値設定

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
	for (auto& fragment : fragment_)
	{
		fragment.toPlayerVec = fragment.oriPos - Player::GetInstance()->GetPos();	//オブジェクトとポリゴンの中心点とのベクトル
		fragment.toPlayerDist = fragment.toPlayerVec.Length();						//オブジェクトとポリゴンの中心点との距離
		//大きくなりすぎないように
		fragment.toPlayerDist = MathUtil::Clamp(fragment.toPlayerDist, 0.f, fragment.maxDist);
		//オブジェクトに近い程大きい値に
		fragment.toPlayerDist = fragment.maxDist - fragment.toPlayerDist;

		fragment.toPlayerVec.Normalize();

		fragment.floatingTimer.RoopReverse();
		NVec3 plusVec = fragment.toPlayerVec * fragment.toPlayerDist;
		plusVec.y = fragment.toPlayerDist * 0.3f;
		if (fragment.toPlayerDist > 0)
		{
			plusVec += NEasing::OutQuad(fragment.floatingTimer.GetTimeRate()) * fragment.toPlayerVec * 0.2f;
		}

		//最終的にプレイヤーから近いほど遠ざかるベクトルを足す
		fragment.obj->position_ = fragment.oriPos + plusVec;
		fragment.obj->rotation_ = plusVec * 360.f;
		fragment.obj->Update();
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
	for (auto& fragment : fragment_)
	{
		fragment.obj->SetBlendMode(BlendMode::None);
		fragment.obj->Draw();
		fragment.obj->SetBlendMode(BlendMode::None);
	}
}

void NTestScene::DrawParticle()
{
}

void NTestScene::DrawForeSprite()
{
}