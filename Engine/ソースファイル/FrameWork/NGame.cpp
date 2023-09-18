#include "NGame.h"
#include "NImGuiManager.h"
#include "imgui.h"

#include "NPostEffect.h"
#include "NSceneChange.h"

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
	NInput::MouseInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//�O���t�B�b�N�X�p�C�v���C��������
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->CreateAll();
#pragma endregion
#pragma region �I�[�f�B�I������
	audio_ = NAudio::GetInstance();
	audio_->Init();
	NAudioManager::AllLoad();
#pragma endregion
#pragma region ���f��������
	NModelManager::AllLoad();
#pragma endregion
#pragma region �e�N�X�`���}�l�[�W���[������
	texManager_ = NTextureManager::GetInstance();
	texManager_->Init();
#pragma endregion
#pragma region �Q�[���V�[��������
	sceneMane_ = NSceneManager::GetInstance();
	sceneMane_->Init();
#pragma endregion
#pragma region ImGui������
	NImGuiManager::GetInstance()->Init();
#pragma endregion
}

void NGame::Update()
{
	NImGuiManager::GetInstance()->Begin();
	//ImGui::ShowDemoWindow();��ImGui�̃T���v�������//

#ifdef _DEBUG //�|�X�g�G�t�F�N�gImGui
#endif // DEBUG

	NFramework::Update();
#pragma region �E�B���h�E���b�Z�[�W����
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX���t���[������
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	if (NPostEffect::GetIsActive())
	{
		NPostEffect::Update();
	}
	sceneMane_->Update();
#pragma endregion
	NImGuiManager::GetInstance()->End();
}

void NGame::Draw()
{
	if (NPostEffect::GetIsActive())
	{
		NPostEffect::PreDrawScene();			//�����_�[�e�N�X�`���̏���(�������ݐ�p��Ԃɂ���)
		sceneMane_->Draw();						//�����_�[�e�N�X�`���ɃQ�[���V�[���̕`��
		NPostEffect::PostDrawScene();			//�ǂݍ��ݐ�p��Ԃɂ��ďI��

		NDX12::GetInstance()->PreDraw();		//�o�b�N�o�b�t�@�̓���ւ�
		NPostEffect::Draw();					//�o�b�N�o�b�t�@�Ƀ|�X�g�G�t�F�N�g�̕`��
		NSceneChange::GetInstance()->Draw();	//�Ö��̓|�X�g�G�t�F�N�g�̏ォ��`�悵����
		NImGuiManager::GetInstance()->Draw();	//ImGui�`��
		NDX12::GetInstance()->PostDraw();		//�o�b�N�o�b�t�@�̂ɕ`�悵���̂�\����
	}
	else
	{
		NDX12::GetInstance()->PreDraw();		//�o�b�N�o�b�t�@�̓���ւ�
		sceneMane_->Draw();						//�Q�[���V�[���̕`��
		NImGuiManager::GetInstance()->Draw();	//ImGui�`��
		NDX12::GetInstance()->PostDraw();		//�o�b�N�o�b�t�@�̂ɕ`�悵���̂�\����
	}
}

void NGame::Finalize()
{
#pragma region ��n��
	audio_->Finalize();
	win_->Finalize();
	NImGuiManager::GetInstance()->Finalize();
#pragma endregion
	NFramework::Finalize();
}
