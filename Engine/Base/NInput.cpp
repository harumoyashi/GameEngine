#include "NInput.h"
#include "NMathUtil.h"
#include <cassert>

//xinput.lib ���C���|�[�g
#pragma comment (lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectInput�̏�����
ComPtr<IDirectInputDevice8> NInput::sKeyboard;
ComPtr<IDirectInput8> NInput::sDirectInput;

// �S�L�[�̓��͏�Ԃ��擾����
static BYTE keys[256] = {};

// �S�L�[��1F�O�̓��͏�Ԃ��擾����
static BYTE prev[256] = {};

NInput* NInput::GetInstance()
{
	static NInput instance;
	return &instance;
}

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
XINPUT_STATE NInput::statePad_{};
XINPUT_STATE NInput::prevPad_{};
//�ڑ�����Ă邩
bool NInput::isConnect_ = false;
//�U��
XINPUT_VIBRATION NInput::vibration_{};

void NInput::PadInit()
{
	ZeroMemory(&statePad_, sizeof(XINPUT_STATE));

	DWORD result;
	result = XInputGetState(0, &statePad_);

	if (result == ERROR_SUCCESS)
	{
		isConnect_ = true;
	}
	else
	{
		isConnect_ = false;
	}
}

void NInput::PadUpdate()
{
	prevPad_ = statePad_;
	DWORD result;
	result = XInputGetState(0, &statePad_);
	if (result == ERROR_SUCCESS)
	{
		isConnect_ = true;
	}
	else
	{
		isConnect_ = false;
	}
	SetDeadZone();
}

bool NInput::IsButton(const uint32_t button)
{
	return statePad_.Gamepad.wButtons == button;
}

bool NInput::IsButtonDown(const uint32_t button)
{
	return statePad_.Gamepad.wButtons == button && prevPad_.Gamepad.wButtons != button;
}

bool NInput::IsButtonRelease(const uint32_t button)
{
	return statePad_.Gamepad.wButtons != button && prevPad_.Gamepad.wButtons == button;
}

uint32_t NInput::GetTrigger(const bool isLeft)
{
	if (isLeft)
	{
		return prevPad_.Gamepad.bLeftTrigger < 128 && statePad_.Gamepad.bLeftTrigger >= 128;
	}
	else
	{
		return prevPad_.Gamepad.bRightTrigger < 128 && statePad_.Gamepad.bRightTrigger >= 128;
	}
}

void NInput::SetDeadZone()
{
	if ((statePad_.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		statePad_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(statePad_.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			statePad_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		statePad_.Gamepad.sThumbLX = 0;
		statePad_.Gamepad.sThumbLY = 0;
	}

	if ((statePad_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		statePad_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(statePad_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			statePad_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		statePad_.Gamepad.sThumbRX = 0;
		statePad_.Gamepad.sThumbRY = 0;
	}
}

NVector2 NInput::GetStick(const bool isLeft)
{
	if (isLeft)
	{
		SHORT x = statePad_.Gamepad.sThumbLX;
		SHORT y = statePad_.Gamepad.sThumbLY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
	else
	{
		SHORT x = statePad_.Gamepad.sThumbRX;
		SHORT y = statePad_.Gamepad.sThumbRY;

		return NVector2(static_cast<float>(x) / 32767.0f, static_cast<float>(y) / 32767.0f);
	}
}

uint32_t NInput::StickTriggered(const bool isVertical, const bool isLstick)
{
	if (isLstick)
	{
		if (isVertical)
		{
			return (statePad_.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				prevPad_.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) -
				(statePad_.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					prevPad_.Gamepad.sThumbLY <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
		else
		{
			return (statePad_.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				prevPad_.Gamepad.sThumbLX <= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) -
				(statePad_.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					prevPad_.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		}
	}
	else
	{
		if (isVertical)
		{
			return (statePad_.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				prevPad_.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) -
				(statePad_.Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					prevPad_.Gamepad.sThumbRY <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
		else
		{
			return (statePad_.Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				prevPad_.Gamepad.sThumbRX <= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) -
				(statePad_.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					prevPad_.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		}
	}
}

void NInput::Vibration(const float leftVibrationPower, const float rightVibrationPower)
{
	float lVibPower = leftVibrationPower, rVibPower = rightVibrationPower;
	lVibPower = MathUtil::Clamp<float>(leftVibrationPower, 0.0f, 1.0f);
	rVibPower = MathUtil::Clamp<float>(rightVibrationPower, 0.0f, 1.0f);

	vibration_.wLeftMotorSpeed = (int)(lVibPower * 65535.0f);
	vibration_.wRightMotorSpeed = (int)(rVibPower * 65535.0f);
	XInputSetState(0, &vibration_);
}
