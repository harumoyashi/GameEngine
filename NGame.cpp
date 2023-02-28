#include "NGame.h"

void NGame::Init()
{
#pragma region WindowsAPI������
	win = NWindows::GetInstance();
	win->Set();
	win->CreateWindowObj();
	win->Display();
#pragma endregion
#pragma region DirectX������
	dx12 = NDX12::GetInstance();
	dx12->Init(win);
	//input������
	NInput::KeyInit(win->GetHInstance(), win->GetHwnd());
	NInput::GetInstance()->PadInit();
	//�O���t�B�b�N�X�p�C�v���C��������
	pipeline = PipeLineManager::GetInstance();
	pipeline->Init();
#pragma endregion
#pragma region �I�[�f�B�I������
	audio = NAudio::GetInstance();
	audio->Init();
	NAudioManager::AllLoad();
#pragma endregion
#pragma region �e�N�X�`���}�l�[�W���[������
	texManager = NTextureManager::GetInstance();
	texManager->Init();
#pragma endregion
#pragma region �Q�[���V�[��������
	sceneMane = NSceneManager::GetInstance();
	sceneMane->Init();
#pragma endregion
}

void NGame::Update()
{
#pragma region �E�B���h�E���b�Z�[�W����
	if (win->WindowMessage()) { isGameEnd = true; }
#pragma endregion
#pragma region DirectX���t���[������
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
#pragma region ��n��
	audio->Finalize();
	win->Finalize();
	sceneMane->Finalize();
#pragma endregion
}
