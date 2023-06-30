#include "NGame.h"
#include "NImGuiManager.h"
#include "imgui.h"

#include "GaussianBlur.h"
#include "RadialBlur.h"

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
	postEffect_ = std::make_unique<NPostEffect>();
	postEffect_->Init();
#pragma endregion
#pragma region ImGui初期化
	NImGuiManager::GetInstance()->Init();
#pragma endregion
	isPostEffect_ = false;
}

void NGame::Update()
{
	NImGuiManager::GetInstance()->Begin();
	//ImGui::ShowDemoWindow();

#ifdef _DEBUG
	ImGui::Begin("PostEffectType");
	static int postEffectNum;
	const char* items[] = {"NoEffect","GaussianBlur","RadianBlur","CG4" };
	if (ImGui::Combo("PostEffect Choice", &postEffectNum, items, IM_ARRAYSIZE(items)))
	{
		switch (postEffectNum)
		{
		case 0:
			isPostEffect_ = false;
			
			break;

		case 1:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<GaussianBlur>();
			postEffect_->Init();

			break;

		case 2:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<RadialBlur>();
			postEffect_->Init();

			break;

		case 3:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<NPostEffect>();
			postEffect_->Init();

			break;

		default:
			break;
		}
	}
	
	ImGui::End();
#endif // DEBUG

	NFramework::Update();
#pragma region ウィンドウメッセージ処理
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX毎フレーム処理
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	//postEffect_->TexChange();
	if (isPostEffect_)
	{
		postEffect_->Update();
	}
	sceneMane_->Update();
#pragma endregion
	NImGuiManager::GetInstance()->End();
}

void NGame::Draw()
{
	if (isPostEffect_)
	{
		postEffect_->PreDrawScene();			//レンダーテクスチャの準備(書き込み専用状態にする)
		sceneMane_->Draw();						//レンダーテクスチャにゲームシーンの描画
		postEffect_->PostDrawScene();			//読み込み専用状態にして終了

		NDX12::GetInstance()->PreDraw();		//バックバッファの入れ替え
		postEffect_->Draw();					//バックバッファにポストエフェクトの描画
		NImGuiManager::GetInstance()->Draw();	//ImGui描画
		NDX12::GetInstance()->PostDraw();		//バックバッファのに描画したのを表示に
	}
	else
	{
		NDX12::GetInstance()->PreDraw();		//バックバッファの入れ替え
		sceneMane_->Draw();						//ゲームシーンの描画
		NImGuiManager::GetInstance()->Draw();	//ImGui描画
		NDX12::GetInstance()->PostDraw();		//バックバッファのに描画したのを表示に
	}
}

void NGame::Finalize()
{
#pragma region 後始末
	audio_->Finalize();
	win_->Finalize();
	NImGuiManager::GetInstance()->Finalize();
#pragma endregion
	NFramework::Finalize();
}
