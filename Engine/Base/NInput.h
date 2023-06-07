#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <Xinput.h>
#include <stdint.h>
#include "NVector2.h"

#include <wrl.h>

class NInput
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	static ComPtr<IDirectInputDevice8> keyboard;
	static ComPtr<IDirectInput8> directInput;

	//�U��
	XINPUT_VIBRATION vibration;

public:
	NInput() {};
	static NInput* GetInstance();

	//input������
	static void KeyInit(HINSTANCE hInstance, HWND hwnd);
	//input�X�V
	static void KeyUpdate();

	//�L�[�{�[�h���͏����p (�Ԃ�l0,1)
	//�������ςȂ�
	static bool IsKey(uint8_t key);	//UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsKeyDown(uint8_t key);
	//�������u��
	static bool IsKeyRelease(uint8_t key);

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
	bool IsButton(uint32_t button);    //UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	bool IsButtonDown(uint32_t button);
	//�������u��
	bool IsButtonRelease(uint32_t button);

	//�g���K�[�̉������݋�擾
	//isLeft:�E���ǂ����I
	uint32_t GetTrigger(bool isLeft = true);

	//�f�b�h�]�[���̐ݒ�
	void SetDeadZone();

	//�X�e�B�b�N�̌X����擾
	//isLeft:�E���ǂ����I
	NVector2 GetStick(bool isLeft = true);

	//isVertical:����������
	//isLstick:L�X�e�B�b�N��
	//��A���͂Ȃ�-1
	//���A�E�Ȃ�+1���Ԃ��Ă���
	uint32_t StickTriggered(bool isVertical,bool isLstick = true);

	//�R���g���[���[�̐U����ݒ�
	//�p���[��0.0f~1.0f�œ��͂��Ă�
	void Vibration(float leftVibrationPower, float rightVibrationPower);
};