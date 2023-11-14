#pragma once
#include "IScene.h"
#include <memory>
#include <wrl.h>

enum Scene {
	TITLESCENE,
	GAMESCENE,
};

class NSceneManager final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static std::unique_ptr<IScene> currentScene_;
	static std::unique_ptr<IScene> nextScene_;

public:
	//インスタンス取得
	static NSceneManager* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();
	void DrawBackSprite();
	void DrawBack3D();
	void Draw3D();
	void DrawParticle();
	void DrawForeSprite();
	//ポストエフェクトかけるものだけ描画
	void DrawBackSpritePE();
	void DrawBack3DPE();
	void Draw3DPE();
	void DrawParticlePE();
	void DrawForeSpritePE();

	//シーンの変更
	template<typename T>
	static void ChangeScene()
	{
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();
		nextScene->Init();
		currentScene_ = std::move(nextScene);
	}

	NSceneManager();

#pragma region ゲッター
#pragma endregion
private:
	NSceneManager(const NSceneManager&) = delete;
	~NSceneManager() = default;
	NSceneManager& operator=(const NSceneManager&) = delete;

};