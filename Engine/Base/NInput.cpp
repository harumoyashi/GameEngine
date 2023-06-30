#include "NInput.h"
#include "NMathUtil.h"
#include <cassert>

//xinput.lib をインポート
#pragma comment (lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

NInput* NInput::GetInstance()
{
	static NInput instance;
	return &instance;
}

//---------こっからマウス------------//
ComPtr<IDirectInputDevice8> NInput::sDevMouse;
DIMOUSESTATE2 NInput::sStateMouse;
DIMOUSESTATE2 NInput::sPrevMouse;

void NInput::MouseInit(const HINSTANCE& hInstance, const HWND& hwnd)
{
	HRESULT result = S_FALSE;

	// DirectInputの初期化
	result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&sDirectInput, nullptr);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成
	result = sDirectInput->CreateDevice(GUID_SysMouse, &sDevMouse, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = sDevMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result =
		sDevMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}


void NInput::MouseUpdate()
{
	sDevMouse->Acquire(); // マウス動作開始

	// 前回の入力を保存
	sPrevMouse = sStateMouse;

	// マウスの入力
	sDevMouse->GetDeviceState(sizeof(sStateMouse), &sStateMouse);
}

// マウスのボタン押下をチェック
// 指定したボタンが押されてるかチェック
bool NInput::PushMouse(const MouseButton button)
{
	// 0でなければ押している
	if (sStateMouse.rgbButtons[button]) {
		return true;
	}

	// 押していない
	return false;
}

// マウスのトリガーをチェック
// 指定したボタンが押されてるかチェック
bool NInput::TriggerMouse(const MouseButton button)
{
	// 前回が0で、今回が0でなければトリガー
	if (!sPrevMouse.rgbButtons[button] && sStateMouse.rgbButtons[button]) {
		return true;
	}

	// トリガーでない
	return false;
}

/// <summary>
/// マウス移動量を取得
/// </summary>
/// <returns>マウス移動量</returns>
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

//マウスの移動量を反映
void NInput::SetMouseMove(NVector2& mouseVec)
{
	sStateMouse.lX = (LONG)mouseVec.x;
	sStateMouse.lY = (LONG)mouseVec.y;
}

//マウスホイールの移動量を反映
void NInput::SetWheelMove(float wheelMove)
{
	sStateMouse.lZ = (LONG)wheelMove;
}

//---------こっからキーボード------------//
ComPtr<IDirectInputDevice8> NInput::sKeyboard;
ComPtr<IDirectInput8> NInput::sDirectInput;
// 全キーの入力状態を取得する
static BYTE keys[256] = {};
// 全キーの1F前の入力状態を取得する
static BYTE prev[256] = {};

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
XINPUT_STATE NInput::sStatePad{};
XINPUT_STATE NInput::sPrevPad{};
//接続されてるか
bool NInput::sIsConnect = false;
//振動
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

//押しっぱなし
bool NInput::IsButton(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons == button;
}

//押した瞬間
bool NInput::IsButtonDown(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons == button && sPrevPad.Gamepad.wButtons != button;
}

//離した瞬間
bool NInput::IsButtonRelease(const uint32_t button)
{
	return sStatePad.Gamepad.wButtons != button && sPrevPad.Gamepad.wButtons == button;
}

//トリガーの押し込み具合取得
//isLeft:右左どっち！
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

//デッドゾーンの設定
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

//スティックの傾き具合取得(0.0f~1.0f)
//isLeft:右左どっち！
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

//isVertical:垂直方向か
//isLstick:Lスティックか
//上、左はなら-1
//下、右なら+1が返ってくる
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

//コントローラーの振動を設定
//パワーは0.0f~1.0fで入力してね
void NInput::Vibration(const float leftVibrationPower, const float rightVibrationPower)
{
	float lVibPower = leftVibrationPower, rVibPower = rightVibrationPower;
	lVibPower = MathUtil::Clamp<float>(leftVibrationPower, 0.0f, 1.0f);
	rVibPower = MathUtil::Clamp<float>(rightVibrationPower, 0.0f, 1.0f);

	sVibration.wLeftMotorSpeed = (int)(lVibPower * 65535.0f);
	sVibration.wRightMotorSpeed = (int)(rVibPower * 65535.0f);
	XInputSetState(0, &sVibration);
}
