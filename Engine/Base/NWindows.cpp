#include "NWindows.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"winmm.lib")

//�����ς���Ɠ{����̂Ő搶�ɕ���
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

NWindows* NWindows::GetInstance()
{
	static NWindows instance;
	return &instance;
}

//�E�B���h�v���V�[�W��
//�ʓ|�����Ǐ����Ȃ��Ⴂ���Ȃ��֐�
LRESULT NWindows::WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd,msg,wparam,lparam))
	{
		return true;
	}

	switch (msg)
	{
		//�E�B���h�E���j�����ꂽ��Ă΂��
	case WM_DESTROY:
		PostQuitMessage(0);	//OS�ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	//����̏������s��
}

//Window�N���X�̐ݒ�
void NWindows::Set()
{
	win_.cbSize = sizeof(WNDCLASSEX);
	win_.lpfnWndProc = (WNDPROC)WindowProc;		//�E�B���h�E�v���V�[�W����ݒ�
	win_.lpszClassName = L"DX12Sample";			//�A�v���P�[�V�����N���X��
	win_.hInstance = GetModuleHandle(nullptr);		//�n���h���̎擾
	win_.hCursor = LoadCursor(NULL, IDC_ARROW);	//�J�[�\���w��

	RegisterClassEx(&win_);	//�A�v���P�[�V�����N���X�i�E�B���h�E�N���X�̎w���OS�ɓ`����j

	wrc_ = { 0,0,sWin_width,sWin_height };	//�E�B���h�E�T�C�Y�����߂�

	//�֐����g���ăE�B���h�E�̃T�C�Y��␳����
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	//�V�X�e���^�C�}�[�̕���x���グ��
	timeBeginPeriod(1);
}

//�R���\�[���ւ̕����o��
void NWindows::DebugText(const std::string& text)
{
	OutputDebugStringA(text.c_str());
}

//�E�B���h�E�I�u�W�F�N�g�̐���
void NWindows::CreateWindowObj()
{
	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(
		win_.lpszClassName,		//�N���X���w��
		L"DX12�e�X�g",			//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	//�^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			//�\��x���W��OS�ɂ��C��
		CW_USEDEFAULT,			//�\��y���W��OS�ɂ��C��
		wrc_.right - wrc_.left,	//�E�B���h�E��
		wrc_.bottom - wrc_.top,	//�E�B���h�E��
		nullptr,				//�e�E�B���h�E�n���h��
		nullptr,				//���j���[�n���h��
		win_.hInstance,			//�Ăяo���A�v���P�[�V�����n���h��
		nullptr					//�ǉ��p�����[�^�[
	);
}

//�E�B���h�E�\��
void NWindows::Display()
{
	ShowWindow(hwnd_, SW_SHOW);
}

bool NWindows::WindowMessage()
{
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);	//�L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg_);	//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	//�A�v���P�[�V�������I���Ƃ���mwssage��WM_QUIT�ɂȂ�
	if (msg_.message == WM_QUIT)
	{
		return true;	//�I�����b�Z�[�W�������烋�[�v�𔲂���
	}

	return false;
}

void NWindows::Finalize()
{
	//�����N���X�͎g��Ȃ��̂œo�^��������
	UnregisterClass(win_.lpszClassName, win_.hInstance);
}