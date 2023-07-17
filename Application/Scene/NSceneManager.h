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
	//�C���X�^���X�擾
	static NSceneManager* GetInstance();

	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�V�[���̕ύX
	template<typename T>
	static void ChangeScene()
	{
		std::unique_ptr<IScene> nextScene = std::make_unique<T>();
		nextScene->Init();
		currentScene_ = std::move(nextScene);
	}

	NSceneManager();

#pragma region �Q�b�^�[
#pragma endregion
private:


};