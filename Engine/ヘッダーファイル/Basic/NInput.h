#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <Xinput.h>
#include <stdint.h>
#include "NVec2.h"
#include "NVec3.h"

#include <wrl.h>

class NInput final
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	struct MouseMove {
		LONG lX;
		LONG lY;
		LONG lZ;	//�����̓}�E�X�z�C�[��
	};
	static ComPtr<IDirectInputDevice8> sDevMouse;
	static DIMOUSESTATE2 sStateMouse;
	static DIMOUSESTATE2 sPrevMouse;

public:
	enum MouseButton
	{
		MouseLeft,
		MouseRight,
		MouseMiddle,
	};

	//mouse������
	static void MouseInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//mouse�X�V
	static void MouseUpdate();

	// �}�E�X�̃{�^���������`�F�b�N
	// �w�肵���{�^����������Ă邩�`�F�b�N
	static bool PushMouse(const MouseButton button = MouseLeft);

	// �}�E�X�̃g���K�[���`�F�b�N
	// �w�肵���{�^����������Ă邩�`�F�b�N
	static bool TriggerMouse(const MouseButton button = MouseLeft);

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	static NVec3 GetMouseMove(bool isNowState = true);

	//�}�E�X�̈ړ��ʂ𔽉f
	static void SetMouseMove(NVec2& mouseVec);

	//�}�E�X�z�C�[���̈ړ��ʂ𔽉f
	static void SetWheelMove(float wheelMove);

private:
	static ComPtr<IDirectInputDevice8> sKeyboard;
	static ComPtr<IDirectInput8> sDirectInput;

public:
	static NInput* GetInstance();

	//key������
	static void KeyInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//key�X�V
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
	static XINPUT_STATE sStatePad;
	static XINPUT_STATE sPrevPad;

	//�ڑ�����Ă邩
	static bool sIsConnect;

	//�U��
	static XINPUT_VIBRATION sVibration;

public:
	//pad������
	void PadInit();
	//pad�X�V
	void PadUpdate();

	//�������ςȂ�
	static bool IsButton(const uint32_t button);    //UINT�̓r�b�g���w�肵����^���R�ۂ�
	//�������u��
	static bool IsButtonDown(const uint32_t button);
	//�������u��
	static bool IsButtonRelease(const uint32_t button);

	//�g���K�[�̉������݋�擾
	//isLeft:�E���ǂ����I
	static uint32_t GetTrigger(bool isLeft = true);

	//�f�b�h�]�[���̐ݒ�
	static void SetDeadZone();

	//�X�e�B�b�N�̌X����擾(0.0f~1.0f)
	//isLeft:�E���ǂ����I
	static NVec2 GetStick(bool isLeft = true);

	//isVertical:����������
	//isLstick:L�X�e�B�b�N��
	//��A���͂Ȃ�-1
	//���A�E�Ȃ�+1���Ԃ��Ă���
	static uint32_t StickTriggered(bool isVertical, bool isLstick = true);

	//�R���g���[���[�̐U����ݒ�
	//�p���[��0.0f~1.0f�œ��͂��Ă�
	static void Vibration(const float leftVibrationPower, const float rightVibrationPower);

	//�ڑ����擾
	static bool GetIsConnect() { return sIsConnect; }

private:
	NInput() = default;
	NInput(const NInput&) = delete;
	~NInput() = default;
	NInput& operator=(const NInput&) = delete;
};