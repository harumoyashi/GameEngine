#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"

#include "Player.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
}

void NGameScene::LoadResources()
{
}

void NGameScene::Init()
{
#pragma region	オーディオ初期化
	audio_ = NAudio::GetInstance();
#pragma endregion
#pragma region	カメラ初期化
	camera_.ProjectiveProjection();
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
#pragma region 描画初期化処理
	//オブジェクト
	Player::GetInstance()->Init();

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

	//タイマーストップフラグ初期化
	isMoveUpdate_ = true;
}

void NGameScene::Update()
{
#pragma region カメラ
	//ビュー行列の再生成
	//右クリックしたらカメラモード切り替わる
	if (NInput::TriggerMouse(NInput::MouseRight))
	{
		camera_.ChangeIsDebugCamera();
	}
	camera_.Update();
	NCamera::sCurrentCamera = &camera_;
#pragma endregion
	//基本動ける
	isMoveUpdate_ = true;
#pragma region プレイヤー
	Player::GetInstance()->Update();
	//プレイヤーが動いてないときは他のものも動けなくする
	if (Player::GetInstance()->GetMoveVelo().Length() < 0.0f)
	{
		isMoveUpdate_ = false;
	}
#pragma endregion

	//ライトたちの更新
	lightGroup_->Update();

	//シーン切り替え
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::ChangeScene<NTitleScene>();
	}
}

void NGameScene::DrawBackSprite()
{
}

void NGameScene::Draw3D()
{
	Player::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
}