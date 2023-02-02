#include "NWindows.h"
#include "NDX12.h"
#include "NGPipeline.h"
#include "NAudio.h"
#include "NTexture.h"
#include "NSceneManager.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma region WindowsAPI������
	NWindows* win = NWindows::GetInstance();
	win->Set();
	win->CreateWindowObj();
	win->Display();
#pragma endregion
#pragma region DirectX������
	NDX12* dx12 = NDX12::GetInstance();
	dx12->Init(win);
	//input������
	NInput::KeyInit(win->GetHInstance(), win->GetHwnd());
	NInput::GetInstance()->PadInit();
	//�O���t�B�b�N�X�p�C�v���C��������
	PipeLineManager* pipeline = PipeLineManager::GetInstance();
	pipeline->Init();
#pragma endregion
#pragma region �I�[�f�B�I������
	NAudio* audio = NAudio::GetInstance();
	audio->Init();
#pragma endregion
#pragma region �e�N�X�`���}�l�[�W���[������
	NTextureManager* texManager = NTextureManager::GetInstance();
	texManager->Init();
#pragma endregion
#pragma region �Q�[���V�[��������
	NSceneManager* sceneMane = NSceneManager::GetInstance();
	sceneMane->Init();
#pragma endregion
	//�Q�[�����[�v
	while (true)
	{
#pragma region �E�B���h�E���b�Z�[�W����
		if (win->WindowMessage()) { break; }
#pragma endregion
#pragma region DirectX���t���[������
		//DirectX���t���[���@��������
		NInput::KeyUpdate();
		NInput::GetInstance()->PadUpdate();
		sceneMane->Update();
		sceneMane->Draw();
	}
#pragma endregion
#pragma region WindowsAPI��n��
	audio->Finalize();
	win->Finalize();
	sceneMane->Finalize();
#pragma endregion
	return 0;
}