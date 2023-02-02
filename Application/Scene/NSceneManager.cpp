#include "NSceneManager.h"
#pragma region static�����o�ϐ�������
//�V�[���̏�����
int NSceneManager::scene = TITLESCENE;
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
}

void NSceneManager::Draw()
{
#pragma region �`��O����
	NPreDraw* preDraw = nullptr;
	preDraw = new NPreDraw();

	preDraw->SetResBarrier();
	preDraw->SetRenderTarget();
	preDraw->ClearScreen();

	preDraw->SetViewport();
	preDraw->SetScissorRect();
#pragma endregion
	//�^�C�g���V�[���̕`�揈��
	if (scene == TITLESCENE) {
		titleScene->Draw();
	}
	//�Q�[���V�[���̕`�揈��
	else if (scene == GAMESCENE) {
		gameScene->Draw();
	}
	NDX12::GetInstance()->PostDraw(preDraw->barrierDesc);
}

void NSceneManager::Finalize()
{
}

void NSceneManager::SetScene(int selectScene)
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
