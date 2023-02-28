#include "NGame.h"

void NGame::Init()
{
#pragma region WindowsAPI初期化
	win = NWindows::GetInstance();
	win->Set();
	win->CreateWindowObj();
	win->Display();
#pragma endregion
#pragma region DirectX初期化
	dx12 = NDX12::GetInstance();
	dx12->Init(win);
	//input初期化
	NInput::KeyInit(win->GetHInstance(), win->GetHwnd());
	NInput::GetInstance()->PadInit();
	//グラフィックスパイプライン初期化
	pipeline = PipeLineManager::GetInstance();
	pipeline->Init();
#pragma endregion
#pragma region オーディオ初期化
	audio = NAudio::GetInstance();
	audio->Init();
	NAudioManager::AllLoad();
#pragma endregion
#pragma region テクスチャマネージャー初期化
	texManager = NTextureManager::GetInstance();
	texManager->Init();
#pragma endregion
#pragma region ゲームシーン初期化
	sceneMane = NSceneManager::GetInstance();
	sceneMane->Init();
#pragma endregion
}

void NGame::Update()
{
#pragma region ウィンドウメッセージ処理
	if (win->WindowMessage()) { isGameEnd = true; }
#pragma endregion
#pragma region DirectX毎フレーム処理
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	sceneMane->Update();
#pragma endregion
}

void NGame::Draw()
{
	sceneMane->Draw();
}

void NGame::Finalize()
{
#pragma region 後始末
	audio->Finalize();
	win->Finalize();
	sceneMane->Finalize();
#pragma endregion
}
