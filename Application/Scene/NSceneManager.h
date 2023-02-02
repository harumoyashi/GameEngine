#pragma once
#include "NTitleScene.h"
#include "NGameScene.h"

enum Scene {
	TITLESCENE,
	GAMESCENE,
};

class NSceneManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NTitleScene* titleScene;
	NGameScene* gameScene;	//�Q�[���V�[��

	static int scene;		//���݂̃V�[��
	static bool isSceneChange;	//�V�[���̏������t���O

public:


public:
	//�C���X�^���X�擾
	static NSceneManager* GetInstance();

	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�I������
	void Finalize();
	//�V�[���̕ύX
	static void SetScene(int selectScene);

	NSceneManager();

#pragma region �Q�b�^�[
#pragma endregion
private:


};