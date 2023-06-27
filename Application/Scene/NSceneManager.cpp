#include "NSceneManager.h"
#include "NImGuiManager.h"
#include "imgui.h"
#pragma region static�����o�ϐ�������
//�V�[���̏�����
uint32_t NSceneManager::scene = TITLESCENE;
//�V�[���ύX�t���O�̏�����
bool NSceneManager::isSceneChange = false;
#pragma region
NSceneManager* NSceneManager::GetInstance()
{
	static NSceneManager instance;
	return &instance;
}

void NSceneManager::Init()
{
#pragma region ImGui������
	NImGuiManager::GetInstance()->Init();
#pragma endregion
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
	NImGuiManager::GetInstance()->Begin();
	//�^�C�g���V�[���̍X�V����
	if (scene == TITLESCENE) {
		titleScene->Update();
	}
	//�Q�[���V�[���̍X�V����
	else if (scene == GAMESCENE) {
		gameScene->Update();
	}

	//�V�[���ύX�����ꂽ��
	if (isSceneChange == true) {
		//�^�C�g���V�[����������
		if (scene == TITLESCENE) {
			//���Z�b�g
			titleScene->Reset();
		}
		//�Q�[���V�[���Ȃ�
		else if (scene == GAMESCENE) {
			//���Z�b�g
			gameScene->Reset();
		}
		//�V�[���ύX�t���OOFF�ɂ���
		isSceneChange = false;
	}

	/*ImGui::Begin("maru");
	ImGui::Text("yoyoyo");
	ImGui::End();
	ImGui::ShowDemoWindow();*/
	NImGuiManager::GetInstance()->End();
}

void NSceneManager::Draw()
{
	//�^�C�g���V�[���̕`�揈��
	if (scene == TITLESCENE) {
		titleScene->Draw();
	}
	//�Q�[���V�[���̕`�揈��
	else if (scene == GAMESCENE) {
		gameScene->Draw();
	}

	NImGuiManager::GetInstance()->Draw();
}

void NSceneManager::Finalize()
{
	NImGuiManager::GetInstance()->Finalize();
}

void NSceneManager::SetScene(uint32_t selectScene)
{
	// --�V�[����ύX-- //
	scene = selectScene;

	// --�V�[���ύX�t���O��ON��-- //
	isSceneChange = true;
}

NSceneManager::NSceneManager()
{
	titleScene = titleScene->GetInstance();
	gameScene = gameScene->GetInstance();
}
