#include "NGame.h"
#include "NImGuiManager.h"
#include "imgui.h"

#include "GaussianBlur.h"
#include "RadialBlur.h"

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
	postEffect_ = std::make_unique<NPostEffect>();
	postEffect_->Init();
#pragma endregion
#pragma region ImGui������
	NImGuiManager::GetInstance()->Init();
#pragma endregion
	isPostEffect_ = false;
}

void NGame::Update()
{
	NImGuiManager::GetInstance()->Begin();
	//ImGui::ShowDemoWindow();

#ifdef _DEBUG
	ImGui::Begin("PostEffectType");
	static int postEffectNum;
	const char* items[] = {"NoEffect","GaussianBlur","RadianBlur","CG4" };
	if (ImGui::Combo("PostEffect Choice", &postEffectNum, items, IM_ARRAYSIZE(items)))
	{
		switch (postEffectNum)
		{
		case 0:
			isPostEffect_ = false;
			
			break;

		case 1:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<GaussianBlur>();
			postEffect_->Init();

			break;

		case 2:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<RadialBlur>();
			postEffect_->Init();

			break;

		case 3:
			isPostEffect_ = true;

			postEffect_ = std::make_unique<NPostEffect>();
			postEffect_->Init();

			break;

		default:
			break;
		}
	}
	
	ImGui::End();
#endif // DEBUG

	NFramework::Update();
#pragma region �E�B���h�E���b�Z�[�W����
	if (win_->WindowMessage()) { NFramework::SetIsGameEnd(true); }
#pragma endregion
#pragma region DirectX���t���[������
	NInput::MouseUpdate();
	NInput::KeyUpdate();
	NInput::GetInstance()->PadUpdate();
	//postEffect_->TexChange();
	if (isPostEffect_)
	{
		postEffect_->Update();
	}
	sceneMane_->Update();
#pragma endregion
	NImGuiManager::GetInstance()->End();
}

void NGame::Draw()
{
	if (isPostEffect_)
	{
		postEffect_->PreDrawScene();			//�����_�[�e�N�X�`���̏���(�������ݐ�p��Ԃɂ���)
		sceneMane_->Draw();						//�����_�[�e�N�X�`���ɃQ�[���V�[���̕`��
		postEffect_->PostDrawScene();			//�ǂݍ��ݐ�p��Ԃɂ��ďI��

		NDX12::GetInstance()->PreDraw();		//�o�b�N�o�b�t�@�̓���ւ�
		postEffect_->Draw();					//�o�b�N�o�b�t�@�Ƀ|�X�g�G�t�F�N�g�̕`��
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
