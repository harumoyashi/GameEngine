#include "NInput.h"
#include "NMathUtil.h"
#include <cassert>

//xinput.lib ���C���|�[�g
#pragma comment (lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

NInput* NInput::GetInstance()
{
	static NInput instance;
	return &instance;
}

//---------��������}�E�X------------//
ComPtr<IDirectInputDevice8> NInput::sDevMouse;
DIMOUSESTATE2 NInput::sStateMouse;
DIMOUSESTATE2 NInput::sPrevMouse;

void NInput::MouseInit(const HINSTANCE& hInstance, const HWND& hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInput�̏�����
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X�̐���
	result = sDirectInput->CreateDevice(GUID_SysMouse, &sDevMouse, NULL);
	assert(SUCCEEDED(result));

	// ���̓f�[�^�`���̃Z�b�g
	result = sDevMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result =
		sDevMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void NInput::MouseUpdate()
{
	sDevMouse->Acquire(); // �}�E�X����J�n

	// �O��̓��͂�ۑ�
	sPrevMouse = sStateMouse;

	// �}�E�X�̓���
	sDevMouse->GetDeviceState(sizeof(sStateMouse), &sStateMouse);
}

bool NInput::PushMouse(const MouseButton button)
{
	// 0�łȂ���Ή����Ă���
	if (sStateMouse.rgbButtons[button]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool NInput::TriggerMouse(const MouseButton button)
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!sPrevMouse.rgbButtons[button] && sStateMouse.rgbButtons[button]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

NVector3 NInput::GetMouseMove(const bool isNowState) {
	NVector3 tmp;
	if (isNowState)
	{
		tmp.x = (float)sStateMouse.lX;
		tmp.y = (float)sStateMouse.lY;
		tmp.z = (float)sStateMouse.lZ;
	}
	else
	{
		tmp.x = (float)sPrevMouse.lX;
		tmp.y = (float)sPrevMouse.lY;
		tmp.z = (float)sPrevMouse.lZ;
	}
	return tmp;
}

void NInput::SetMouseMove(NVector2& mouseVec)
{
	sStateMouse.lX = (LONG)mouseVec.x;
	sStateMouse.lY = (LONG)mouseVec.y;
}

void NInput::SetWheelMove(float wheelMove)
{
	sStateMouse.lZ = (LONG)wheelMove;
}

//---------��������L�[�{�[�h------------//
ComPtr<IDirectInputDevice8> NInput::sKeyboard;
ComPtr<IDirectInput8> NInput::sDirectInput;
// �S�L�[�̓��͏�Ԃ��擾����
static BYTE keys[256] = {};
// �S�L�[��1F�O�̓��͏�Ԃ��擾����
static BYTE prev[256] = {};

void NInput::KeyInit(const HINSTANCE& hInstance, const HWND& hwnd)
{
	HRESULT result;

	// DirectInput�̏�����
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	result = sDirectInput->CreateDevice(GUID_SysKeyboard, &sKeyboard, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = sKeyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = sKeyboard->SetCooperativeLevel(
		//DISCL_FOREGROUND�F��ʂ���O�ɂ���ꍇ�̂ݓ��͂��󂯕t����
		//DISCL_NONEXCLUSIVE�F�f�o�C�X�����̃A�v�������Ő�L���Ȃ�
		//DISCL_NOWINKEY�FWindows�L�[�𖳌��ɂ���
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void NInput::KeyUpdate()
{
	//1F�O�̃L�[���Ɍ��݂̃L�[�������Ă��火
	memcpy(prev, keys, sizeof(keys));

	//���݂̃L�[���̎擾�J�n
	sKeyboard->Acquire();
	sKeyboard->GetDeviceState(sizeof(keys), keys);
}

//�������ςȂ�
bool NInput::IsKey(const uint8_t key)
{
	return keys[key];
}

//�������u��
bool NInput::IsKeyDown(const uint8_t key)
{
	return keys[key] && !prev[key];
}

//�������u��
bool NInput::IsKeyRelease(const uint8_t key)
{
	return !keys[key] && prev[key];
}

//---------��������p�b�h------------//
//XINPUT_STATE �\���̂̃C���X�^���X���쐬
XINPUT_STATE NInput::sStatePad{};
XINPUT_STATE NInput::sPrevPad{};
//�ڑ�����Ă邩
bool NInput::sIsConnect = false;
//�U��
XINPUT_VIBRATION NInput::sVibration{};

void NInput::PadInit()
{
	ZeroMemory(&sStatePad, sizeof(XINPUT_STATE));

	DWORD result;
	result = XInputGetState(0, &sStatePad);

	if (result == ERROR_SUCCESS)
	{
		sIsConnect = true;
	}
	else
	{
		sIsConnect = false;
	}
}

void NInput::PadUpdate()
{
	sPrevPad = sStatePad;
	DWORD result;
	result = XInputGetState(0, &sStatePad);
	if (result == ERROR_SUCCESS)
	{
		sIsConnect = true;
	}
	else
	{
		sIsConnect = false;
	}
	SetDeadZone();
}

bool NInput::IsButton(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons == button;
}

bool NInput::IsButtonDown(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons == button && sPrevPad.Gamepad.wButtons != button;
}

bool NInput::IsButtonRelease(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons != button && sPrevPad.Gamepad.wButtons == button;
}

uint32_t NInput::GetTrigger(const bool isLeft)
{
	if (isLeft)
	{
		return sPrevPad.Gamepad.bLeftTrigger < 128 && sStatePad.Gamepad.bLeftTrigger >= 128;
	}
	else
	{
		return sPrevPad.Gamepad.bRightTrigger < 128 && sStatePad.Gamepad.bRightTrigger >= 128;
	}
}

void NInput::SetDeadZone()
{
	if ((sStatePad.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		sStatePad.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(sStatePad.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			sStatePad.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		sStatePad.Gamepad.sThumbLX = 0;
		sStatePad.Gamepad.sThumbLY = 0;
	}

	if ((sStatePad.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		sStatePad.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(sStatePad.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			sStatePad.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		sStatePad.Gamepad.sThumbRX = 0;
		sStatePad.Gamepad.sThumbRY = 0;
	}
}

NVector2 NInput::GetStick(const bool isLeft)
{
	if (isLeft)
	{
		SHORT x = sStatePad.Gamepad.sThumbLX;
		SHORT y = sStatePad.Gamepad.sThumbLY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
	else
	{
		SHORT x = sStatePad.Gamepad.sThumbRX;
		SHORT y = sStatePad.Gamepad.sThumbRY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
}

uint32_t NInput::StickTriggered(const bool isVertical, const bool isLstick)
{
	if (isLstick)
	{
		if (isVertical)
		{
			return (sStatePad.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				sPrevPad.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) -
				(sStatePad.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					sPrevPad.Gamepad.sThumbLY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else
		{
			return (sStatePad.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				sPrevPad.Gamepad.sThumbLX <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) -
				(sStatePad.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					sPrevPad.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
	}
	else
	{
		if (isVertical)
		{
			return (sStatePad.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				sPrevPad.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) -
				(sStatePad.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					sPrevPad.Gamepad.sThumbRY <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else
		{
			return (sStatePad.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				sPrevPad.Gamepad.sThumbRX <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) -
				(sStatePad.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					sPrevPad.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
	}
}

void NInput::Vibration(const float leftVibrationPower, const float rightVibrationPower)
{
	float lVibPower = leftVibrationPower, rVibPower = rightVibrationPower;
	lVibPower = MathUtil::Clamp<float>(leftVibrationPower, 0.0f, 1.0f);
	rVibPower = MathUtil::Clamp<float>(rightVibrationPower, 0.0f, 1.0f);

	sVibration.wLeftMotorSpeed = (int)(lVibPower * 65535.0f);
	sVibration.wRightMotorSpeed = (int)(rVibPower * 65535.0f);
	XInputSetState(0, &sVibration);
}
