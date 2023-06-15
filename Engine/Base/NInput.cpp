#include "NInput.h"
#include "NMathUtil.h"
#include <cassert>

//xinput.lib をインポート
#pragma comment (lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectInputの初期化
ComPtr<IDirectInputDevice8> NInput::sKeyboard;
ComPtr<IDirectInput8> NInput::sDirectInput;

// 全キーの入力状態を取得する
static BYTE keys[256] = {};

// 全キーの1F前の入力状態を取得する
static BYTE prev[256] = {};

NInput* NInput::GetInstance()
{
	static NInput instance;
	return &instance;
}

void NInput::KeyInit(const HINSTANCE& hInstance, const HWND& hwnd)
{
	HRESULT result;

	// DirectInputの初期化
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));
	// キーボードデバイスの生成
	result = sDirectInput->CreateDevice(GUID_SysKeyboard, &sKeyboard, NULL);
	assert(SUCCEEDED(result));
	// 入力データ形式のセット
	result = sKeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = sKeyboard->SetCooperativeLevel(
		//DISCL_FOREGROUND：画面が手前にある場合のみ入力を受け付ける
		//DISCL_NONEXCLUSIVE：デバイスをこのアプリだけで専有しない
		//DISCL_NOWINKEY：Windowsキーを無効にする
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void NInput::KeyUpdate()
{
	//1F前のキー情報に現在のキー情報を入れてから↓
	memcpy(prev, keys, sizeof(keys));

	//現在のキー情報の取得開始
	sKeyboard->Acquire();
	sKeyboard->GetDeviceState(sizeof(keys), keys);
}

//押しっぱなし
bool NInput::IsKey(const uint8_t key)
{
	return keys[key];
}

//押した瞬間
bool NInput::IsKeyDown(const uint8_t key)
{
	return keys[key] && !prev[key];
}

//離した瞬間
bool NInput::IsKeyRelease(const uint8_t key)
{
	return !keys[key] && prev[key];
}

//---------こっからパッド------------//
//XINPUT_STATE 構造体のインスタンスを作成
XINPUT_STATE NInput::statePad_{};
XINPUT_STATE NInput::prevPad_{};
//接続されてるか
bool NInput::isConnect_ = false;
//振動
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
