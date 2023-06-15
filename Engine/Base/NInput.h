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
	static ComPtr<IDirectInputDevice8> sKeyboard;
	static ComPtr<IDirectInput8> sDirectInput;

public:
	NInput() {};
	static NInput* GetInstance();

	//input������
	static void KeyInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//input�X�V
	static void KeyUpdate();

	//�L�[�{�[�h���͏����p (�Ԃ�l0,1)
	//�������ςȂ�
	static bool IsKey(const uint8_t key);	//UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsKeyDown(const uint8_t key);
	//�������u��
	static bool IsKeyRelease(const uint8_t key);

private:
	//XINPUT_STATE �\���̂̃C���X�^���X���쐬
	static XINPUT_STATE statePad_;
	static XINPUT_STATE prevPad_;

	//�ڑ�����Ă邩
	static bool isConnect_;

	//�U��
	static XINPUT_VIBRATION vibration_;

public:
	//������
	void PadInit();
	//�X�V
	void PadUpdate();

	//�������ςȂ�
	static bool IsButton(const uint32_t button);    //UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsButtonDown(const uint32_t button);
	//�������u��
	static bool IsButtonRelease(const uint32_t button);

	//�g���K�[�̉������݋�擾
	//isLeft:�E���ǂ����I
	static uint32_t GetTrigger(const bool isLeft = true);

	//�f�b�h�]�[���̐ݒ�
	static void SetDeadZone();

	//�X�e�B�b�N�̌X����擾
	//isLeft:�E���ǂ����I
	static NVector2 GetStick(const bool isLeft = true);

	//isVertical:����������
	//isLstick:L�X�e�B�b�N��
	//��A���͂Ȃ�-1
	//���A�E�Ȃ�+1���Ԃ��Ă���
	static uint32_t StickTriggered(bool isVertical, const bool isLstick = true);

	//�R���g���[���[�̐U����ݒ�
	//�p���[��0.0f~1.0f�œ��͂��Ă�
	static void Vibration(const float leftVibrationPower, const float rightVibrationPower);

	//�ڑ����擾
	inline static bool GetIsConnect() { return isConnect_; }
};