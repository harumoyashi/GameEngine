#include "NGame.h"

void NGame::Init()
{
	NFramework::Init();

#pragma region WindowsAPI初期化
	win_ = NWindows::GetInstance();
	win_->Set();
	win_->CreateWindowObj();
	win_->Display();
#pragma endregion
#pragma region DirectX初期化
	dx12_ = NDX12::GetInstance();
	dx12_->Init(win_);
	//input初期化
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//グラフィックスパイプライン初期化
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->Init();
#pragma endregion
#pragma region オーディオ初期化
	audio__ = NAudio::GetInstance();
	audio__->Init();
	NAudioManager::AllLoad();
#pragma endregion
#pragma region テクスチャマネージャー初期化
	texManager_ = NTextureManager::GetInstance();
	texManager_->Init();
#pragma endregion
#pragma region ゲームシーン初期化
	sceneMane_ = NSceneManager::GetInstance();
	sceneMane_->Init();
#pragma endregion
}

void NGame::Update()
{
	NFramework::Update();
#pragma region ウィンドウメッセージ処理
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX毎フレーム処理
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	sceneMane_->Update();
#pragma endregion
}

void NGame::Draw()
{
	sceneMane_->Draw();
}

void NGame::Finalize()
{
#pragma region 後始末
	audio__->Finalize();
	win_->Finalize();
	sceneMane_->Finalize();
#pragma endregion
	NFramework::Finalize();
}
