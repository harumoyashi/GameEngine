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
	NInput::MouseInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//グラフィックスパイプライン初期化
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->Init();
#pragma endregion
#pragma region オーディオ初期化
	audio_ = NAudio::GetInstance();
	audio_->Init();
	NAudioManager::AllLoad();
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
#pragma region ポストエフェクト初期化
	postEffect = std::make_unique<NPostEffect>();
	postEffect->Init();
	//postEffect->CreateSprite();
	//postEffect->SetPos(200,300);
#pragma endregion
}

void NGame::Update()
{
	NFramework::Update();
#pragma region ウィンドウメッセージ処理
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX毎フレーム処理
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	postEffect->Update();
	sceneMane_->Update();
#pragma endregion
}

void NGame::Draw()
{
	postEffect->PreDrawScene();			//レンダーテクスチャの準備(書き込み専用状態にする)
	sceneMane_->Draw();					//レンダーテクスチャにゲームシーンの描画
	postEffect->PostDrawScene();		//読み込み専用状態にして終了

	NDX12::GetInstance()->PreDraw();	//バックバッファの入れ替え
	postEffect->Draw();					//バックバッファにポストエフェクトの描画
	NDX12::GetInstance()->PostDraw();	//バックバッファのに描画したのを表示に
}

void NGame::Finalize()
{
#pragma region 後始末
	audio_->Finalize();
	win_->Finalize();
	sceneMane_->Finalize();
#pragma endregion
	NFramework::Finalize();
}
