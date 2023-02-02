#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <Xinput.h>
#include "NVector2.h"

#include <wrl.h>

class NInput
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	static ComPtr<IDirectInputDevice8> keyboard;
	static ComPtr<IDirectInput8> directInput;

public:
	NInput() {};
	static NInput* GetInstance();

	//input������
	static void KeyInit(HINSTANCE hInstance, HWND hwnd);
	//input�X�V
	static void KeyUpdate();

	//�L�[�{�[�h���͏����p (�Ԃ�l0,1)
	//�������ςȂ�
	static bool IsKey(UINT8 key);	//UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsKeyDown(UINT8 key);
	//�������u��
	static bool IsKeyRelease(UINT8 key);

public:
	//XINPUT_STATE �\���̂̃C���X�^���X���쐬
	XINPUT_STATE statePad{};
	XINPUT_STATE prevPad{};

	//�ڑ�����Ă邩
	bool isConnect = false;

public:
	//������
	void PadInit();
	//�X�V
	void PadUpdate();

	//�������ςȂ�
	bool IsButton(UINT button);    //UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	bool IsButtonDown(UINT button);
	//�������u��
	bool IsButtonRelease(UINT button);

	//�g���K�[�̉������݋�擾
	//isLeft:�E���ǂ����I
	int GetTrigger(bool isLeft = true);

	//�f�b�h�]�[���̐ݒ�
	void SetDeadZone();

	//�X�e�B�b�N�̌X����擾
	//isLeft:�E���ǂ����I
	NVector2 GetStick(bool isLeft = true);

	//�X�e�B�b�N����ɓ|�����u�Ԃ�
	//isLeft:�E���ǂ����I
	bool IsStickUp(bool isLeft = true);
	//�X�e�B�b�N�����ɓ|�����u�Ԃ�
	//isLeft:�E���ǂ����I
	bool IsStickDown(bool isLeft = true);
};