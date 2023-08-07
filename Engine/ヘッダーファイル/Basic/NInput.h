#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
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
		LONG lZ;	//ここはマウスホイール
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

	//mouse初期化
	static void MouseInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//mouse更新
	static void MouseUpdate();

	// マウスのボタン押下をチェック
	// 指定したボタンが押されてるかチェック
	static bool PushMouse(const MouseButton button = MouseLeft);

	// マウスのトリガーをチェック
	// 指定したボタンが押されてるかチェック
	static bool TriggerMouse(const MouseButton button = MouseLeft);

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	static NVec3 GetMouseMove(bool isNowState = true);

	//マウスの移動量を反映
	static void SetMouseMove(NVec2& mouseVec);

	//マウスホイールの移動量を反映
	static void SetWheelMove(float wheelMove);

private:
	static ComPtr<IDirectInputDevice8> sKeyboard;
	static ComPtr<IDirectInput8> sDirectInput;

public:
	static NInput* GetInstance();

	//key初期化
	static void KeyInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//key更新
	static void KeyUpdate();

	//キーボード入力処理用 (返り値0,1)
	//押しっぱなし
	static bool IsKey(const uint8_t key);	//UINTはビット数指定したら型自由ぽい
	//押した瞬間
	static bool IsKeyDown(const uint8_t key);
	//離した瞬間
	static bool IsKeyRelease(const uint8_t key);

private:
	//XINPUT_STATE 構造体のインスタンスを作成
	static XINPUT_STATE sStatePad;
	static XINPUT_STATE sPrevPad;

	//接続されてるか
	static bool sIsConnect;

	//振動
	static XINPUT_VIBRATION sVibration;

public:
	//pad初期化
	void PadInit();
	//pad更新
	void PadUpdate();

	//押しっぱなし
	static bool IsButton(const uint32_t button);    //UINTはビット数指定したら型自由ぽい
	//押した瞬間
	static bool IsButtonDown(const uint32_t button);
	//離した瞬間
	static bool IsButtonRelease(const uint32_t button);

	//トリガーの押し込み具合取得
	//isLeft:右左どっち！
	static uint32_t GetTrigger(bool isLeft = true);

	//デッドゾーンの設定
	static void SetDeadZone();

	//スティックの傾き具合取得(0.0f~1.0f)
	//isLeft:右左どっち！
	static NVec2 GetStick(bool isLeft = true);

	//isVertical:垂直方向か
	//isLstick:Lスティックか
	//上、左はなら-1
	//下、右なら+1が返ってくる
	static uint32_t StickTriggered(bool isVertical, bool isLstick = true);

	//コントローラーの振動を設定
	//パワーは0.0f~1.0fで入力してね
	static void Vibration(const float leftVibrationPower, const float rightVibrationPower);

	//接続情報取得
	static bool GetIsConnect() { return sIsConnect; }

private:
	NInput() = default;
	NInput(const NInput&) = delete;
	~NInput() = default;
	NInput& operator=(const NInput&) = delete;
};