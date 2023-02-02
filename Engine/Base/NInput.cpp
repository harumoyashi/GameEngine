#include "NInput.h"
#include <cassert>

//xinput.lib ���C���|�[�g
#pragma comment (lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectInput�̏�����
ComPtr<IDirectInputDevice8> NInput::keyboard;
ComPtr<IDirectInput8> NInput::directInput;

// �S�L�[�̓��͏�Ԃ��擾����
static BYTE keys[256] = {};

// �S�L�[��1F�O�̓��͏�Ԃ��擾����
static BYTE prev[256] = {};

NInput* NInput::GetInstance()
{
	static NInput instance;
	return &instance;
}

void NInput::KeyInit(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	// DirectInput�̏�����
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	// �L�[�{�[�h�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	// ���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard); // �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
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
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(keys), keys);
}

//�������ςȂ�
bool NInput::IsKey(UINT8 key)
{
	return keys[key];
}

//�������u��
bool NInput::IsKeyDown(UINT8 key)
{
	return keys[key] && !prev[key];
}

//�������u��
bool NInput::IsKeyRelease(UINT8 key)
{
	return !keys[key] && prev[key];
}

void NInput::PadInit()
{
	ZeroMemory(&statePad, sizeof(XINPUT_STATE));

	DWORD result;
	result = XInputGetState(0, &statePad);

	if (result == ERROR_SUCCESS)
	{
		isConnect = true;
	}
	else
	{
		isConnect = false;
	}
}

void NInput::PadUpdate()
{
	prevPad = statePad;
	DWORD result;
	result = XInputGetState(0, &statePad);
	if (result == ERROR_SUCCESS)
	{
		isConnect = true;
	}
	else
	{
		isConnect = false;
	}
	SetDeadZone();
}

bool NInput::IsButton(UINT button)
{
	return statePad.Gamepad.wButtons == button;
}

bool NInput::IsButtonDown(UINT button)
{
	return statePad.Gamepad.wButtons == button && prevPad.Gamepad.wButtons != button;
}

bool NInput::IsButtonRelease(UINT button)
{
	return statePad.Gamepad.wButtons != button && prevPad.Gamepad.wButtons == button;
}

int NInput::GetTrigger(bool isLeft)
{
	if (isLeft)
	{
		return prevPad.Gamepad.bLeftTrigger < 128 && statePad.Gamepad.bLeftTrigger >= 128;
	}
	else
	{
		return prevPad.Gamepad.bRightTrigger < 128 && statePad.Gamepad.bRightTrigger >= 128;
	}
}

void NInput::SetDeadZone()
{
	if ((statePad.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		statePad.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(statePad.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			statePad.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		statePad.Gamepad.sThumbLX = 0;
		statePad.Gamepad.sThumbLY = 0;
	}

	if ((statePad.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		statePad.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(statePad.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			statePad.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		statePad.Gamepad.sThumbRX = 0;
		statePad.Gamepad.sThumbRY = 0;
	}
}

NVector2 NInput::GetStick(bool isLeft)
{
	if (isLeft)
	{
		SHORT x = statePad.Gamepad.sThumbLX;
		SHORT y = statePad.Gamepad.sThumbLY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
	else
	{
		SHORT x = statePad.Gamepad.sThumbRX;
		SHORT y = statePad.Gamepad.sThumbRY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
}

bool NInput::IsStickUp(bool isLeft)
{
	if (isLeft)
	{
		return prevPad.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
			statePad.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	}
	else
	{
		return prevPad.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE&&
			statePad.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	}
}

bool NInput::IsStickDown(bool isLeft)
{
	if (isLeft)
	{
		return prevPad.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			statePad.Gamepad.sThumbLY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	}
	else
	{
		return prevPad.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			statePad.Gamepad.sThumbRY <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	}
}
