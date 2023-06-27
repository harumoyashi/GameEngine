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
	NInput::MouseInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::KeyInit(win_->GetHInstance(), win_->GetHwnd());
	NInput::GetInstance()->PadInit();
	//�O���t�B�b�N�X�p�C�v���C��������
	pipeline_ = PipeLineManager::GetInstance();
	pipeline_->Init();
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
#pragma region �|�X�g�G�t�F�N�g������
	postEffect = std::make_unique<NPostEffect>();
	postEffect->Init();
	//postEffect->CreateSprite();
	//postEffect->SetPos(200,300);
#pragma endregion
}

void NGame::Update()
{
	NFramework::Update();
#pragma region �E�B���h�E���b�Z�[�W����
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX���t���[������
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	postEffect->Update();
	sceneMane_->Update();
#pragma endregion
}

void NGame::Draw()
{
	postEffect->PreDrawScene();			//�����_�[�e�N�X�`���̏���(�������ݐ�p��Ԃɂ���)
	sceneMane_->Draw();					//�����_�[�e�N�X�`���ɃQ�[���V�[���̕`��
	postEffect->PostDrawScene();		//�ǂݍ��ݐ�p��Ԃɂ��ďI��

	NDX12::GetInstance()->PreDraw();	//�o�b�N�o�b�t�@�̓���ւ�
	postEffect->Draw();					//�o�b�N�o�b�t�@�Ƀ|�X�g�G�t�F�N�g�̕`��
	NDX12::GetInstance()->PostDraw();	//�o�b�N�o�b�t�@�̂ɕ`�悵���̂�\����
}

void NGame::Finalize()
{
#pragma region ��n��
	audio_->Finalize();
	win_->Finalize();
	sceneMane_->Finalize();
#pragma endregion
	NFramework::Finalize();
}
