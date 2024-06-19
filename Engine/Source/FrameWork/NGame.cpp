#include "NGame.h"
#include "NImGuiManager.h"
#include "imgui.h"

#include "IPostEffect.h"
#include "NSceneChange.h"
#include "GaussianBlur.h"
#include "RadialBlur.h"
#include "NAudioManager.h"

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
	NInput::MouseInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//グラフィックスパイプライン初期化
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->CreateAll();
#pragma endregion
#pragma region オーディオ初期化
	audio_ = NAudio::GetInstance();
	audio_->Init();
	NAudioManager::GetInstance()->AllLoad();
#pragma endregion
#pragma region モデル初期化
	NModelManager::AllLoad();
#pragma endregion
#pragma region テクスチャマネージャー初期化
	texManager_ = NTextureManager::GetInstance();
	texManager_->Init();
#pragma endregion
#pragma region ゲームシーン初期化
	sceneMane_ = NSceneManager::GetInstance();
	sceneMane_->Init();
#pragma endregion
#pragma region ImGui初期化
	NImGuiManager::GetInstance()->Init();
#pragma endregion
}

void NGame::Update()
{
	NImGuiManager::GetInstance()->Begin();
	//ImGui::ShowDemoWindow();でImGuiのサンプル見れる//

	NFramework::Update();
#pragma region ウィンドウメッセージ処理
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX毎フレーム処理
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	if (IPostEffect::GetIsActive())
	{
		IPostEffect::Update();
	}
	sceneMane_->Update();
#pragma endregion
	NImGuiManager::GetInstance()->End();
}

void NGame::Draw()
{
	NDX12::GetInstance()->PreDraw();		//バックバッファの入れ替え
	sceneMane_->Draw();						//ゲームシーンの描画
	NImGuiManager::GetInstance()->Draw();	//ImGui描画
	NDX12::GetInstance()->PostDraw();		//バックバッファのに描画したのを表示に
}

void NGame::Finalize()
{
#pragma region 後始末
	NAudioManager::GetInstance()->SaveVolume();
	audio_->Finalize();
	win_->Finalize();
	NImGuiManager::GetInstance()->Finalize();
#pragma endregion
	NFramework::Finalize();
}
