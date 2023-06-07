#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
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

	//振動
	XINPUT_VIBRATION vibration;

public:
	NInput() {};
	static NInput* GetInstance();

	//input初期化
	static void KeyInit(HINSTANCE hInstance, HWND hwnd);
	//input更新
	static void KeyUpdate();

	//キーボード入力処理用 (返り値0,1)
	//押しっぱなし
	static bool IsKey(uint8_t key);	//UINTはビット数指定したら型自由ぽい
	//押した瞬間
	static bool IsKeyDown(uint8_t key);
	//離した瞬間
	static bool IsKeyRelease(uint8_t key);

public:
	//XINPUT_STATE 構造体のインスタンスを作成
	XINPUT_STATE statePad{};
	XINPUT_STATE prevPad{};

	//接続されてるか
	bool isConnect = false;

public:
	//初期化
	void PadInit();
	//更新
	void PadUpdate();

	//押しっぱなし
	bool IsButton(uint32_t button);    //UINTはビット数指定したら型自由ぽい
	//押した瞬間
	bool IsButtonDown(uint32_t button);
	//離した瞬間
	bool IsButtonRelease(uint32_t button);

	//トリガーの押し込み具合取得
	//isLeft:右左どっち！
	uint32_t GetTrigger(bool isLeft = true);

	//デッドゾーンの設定
	void SetDeadZone();

	//スティックの傾き具合取得
	//isLeft:右左どっち！
	NVector2 GetStick(bool isLeft = true);

	//isVertical:垂直方向か
	//isLstick:Lスティックか
	//上、左はなら-1
	//下、右なら+1が返ってくる
	uint32_t StickTriggered(bool isVertical,bool isLstick = true);

	//コントローラーの振動を設定
	//パワーは0.0f~1.0fで入力してね
	void Vibration(float leftVibrationPower, float rightVibrationPower);
};