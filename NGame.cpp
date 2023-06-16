#include "NGame.h"

void NGame::Init()
{
	NFramework::Init();

#pragma region WindowsAPI������
	win_ = NWindows::GetInstance();
	win_->Set();
	win_->CreateWindowObj();
	win_->Display();
#pragma endregion
#pragma region DirectX������
	dx12_ = NDX12::GetInstance();
	dx12_->Init(win_);
	//input������
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//�O���t�B�b�N�X�p�C�v���C��������
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->Init();
#pragma endregion
#pragma region �I�[�f�B�I������
	audio__ = NAudio::GetInstance();
	audio__->Init();
	NAudioManager::AllLoad();
#pragma endregion
#pragma region �e�N�X�`���}�l�[�W���[������
	texManager_ = NTextureManager::GetInstance();
	texManager_->Init();
#pragma endregion
#pragma region �Q�[���V�[��������
	sceneMane_ = NSceneManager::GetInstance();
	sceneMane_->Init();
#pragma endregion
}

void NGame::Update()
{
	NFramework::Update();
#pragma region �E�B���h�E���b�Z�[�W����
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX���t���[������
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	sceneMane_->Update();
#pragma endregion
}

void NGame::Draw()
{
	sceneMane_->Draw();
}

void NGame::Finalize()
{
#pragma region ��n��
	audio__->Finalize();
	win_->Finalize();
	sceneMane_->Finalize();
#pragma endregion
	NFramework::Finalize();
}
