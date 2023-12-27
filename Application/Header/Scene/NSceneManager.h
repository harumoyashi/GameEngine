#pragma once
#include "IScene.h"
#include <memory>
#include <wrl.h>

// シーン管理 //

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
	//背景スプライト描画
	void DrawBackSprite();
	//背景オブジェクト描画
	void DrawBack3D();
	//前景オブジェクト描画
	void Draw3D();
	//パーティクル描画
	void DrawParticle();
	//前景スプライト描画
	void DrawForeSprite();

	//ポストエフェクトかけるものだけ描画
	//背景スプライト描画
	void DrawBackSpritePE();
	//背景オブジェクト描画
	void DrawBack3DPE();
	//前景オブジェクト描画
	void Draw3DPE();
	//パーティクル描画
	void DrawParticlePE();
	//前景スプライト描画
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