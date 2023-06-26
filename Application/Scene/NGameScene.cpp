#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"

NGameScene* NGameScene::GetInstance()
{
	static NGameScene instance;
	return &instance;
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
}

void NGameScene::Update()
{
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneManager::SetScene(TITLESCENE);
	}
#pragma region 行列の計算
	//ビュー行列の再生成
	camera_.CreateMatView();
	NCamera::sCurrentCamera = &camera_;

#pragma endregion
	//ライトたちの更新
	lightGroup_->Update();
}

void NGameScene::Draw()
{
#pragma region グラフィックスコマンド
	//背景スプライト
	NSprite::CommonBeginDraw();

	//3Dオブジェクト
	NObj3d::CommonBeginDraw();
	
	//前景スプライト
	NSprite::CommonBeginDraw();

	// 4.描画コマンドここまで
#pragma endregion
}

void NGameScene::Reset()
{
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());
}

void NGameScene::Finalize()
{
}