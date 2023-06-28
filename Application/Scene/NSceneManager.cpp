#include "NSceneManager.h"
#pragma region staticメンバ変数初期化
//シーンの初期化
uint32_t NSceneManager::scene = TITLESCENE;
//シーン変更フラグの初期化
bool NSceneManager::isSceneChange = false;
#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Init()
{
	titleScene->Init();
	gameScene->Init();
	if (scene == TITLESCENE)
	{
		titleScene->Reset();
	}
	else if (scene == GAMESCENE)
	{
		gameScene->Reset();
	}
}

void NSceneManager::Update()
{
	//タイトルシーンの更新処理
	if (scene == TITLESCENE) {
		titleScene->Update();
	}
	//ゲームシーンの更新処理
	else if (scene == GAMESCENE) {
		gameScene->Update();
	}

	//シーン変更がされたら
	if (isSceneChange == true) {
		//タイトルシーンだったら
		if (scene == TITLESCENE) {
			//リセット
			titleScene->Reset();
		}
		//ゲームシーンなら
		else if (scene == GAMESCENE) {
			//リセット
			gameScene->Reset();
		}
		//シーン変更フラグOFFにする
		isSceneChange = false;
	}

	/*ImGui::Begin("maru");
	ImGui::Text("yoyoyo");
	ImGui::End();
	ImGui::ShowDemoWindow();*/
}

void NSceneManager::Draw()
{
	//タイトルシーンの描画処理
	if (scene == TITLESCENE) {
		titleScene->Draw();
	}
	//ゲームシーンの描画処理
	else if (scene == GAMESCENE) {
		gameScene->Draw();
	}
}

void NSceneManager::Finalize()
{
}

void NSceneManager::SetScene(uint32_t selectScene)
{
	// --シーンを変更-- //
	scene = selectScene;

	// --シーン変更フラグをONに-- //
	isSceneChange = true;
}

NSceneManager::NSceneManager()
{
	titleScene = titleScene->GetInstance();
	gameScene = gameScene->GetInstance();
}
