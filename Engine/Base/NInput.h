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
	static ComPtr<IDirectInputDevice8> sKeyboard;
	static ComPtr<IDirectInput8> sDirectInput;

public:
	NInput() {};
	static NInput* GetInstance();

	//input初期化
	static void KeyInit(const HINSTANCE& hInstance, const HWND& hwnd);
	//input更新
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
	static XINPUT_STATE statePad_;
	static XINPUT_STATE prevPad_;

	//接続されてるか
	static bool isConnect_;

	//振動
	static XINPUT_VIBRATION vibration_;

public:
	//初期化
	void PadInit();
	//更新
	void PadUpdate();

	//押しっぱなし
	static bool IsButton(const uint32_t button);    //UINTはビット数指定したら型自由ぽい
	//押した瞬間
	static bool IsButtonDown(const uint32_t button);
	//離した瞬間
	static bool IsButtonRelease(const uint32_t button);

	//トリガーの押し込み具合取得
	//isLeft:右左どっち！
	static uint32_t GetTrigger(const bool isLeft = true);

	//デッドゾーンの設定
	static void SetDeadZone();

	//スティックの傾き具合取得
	//isLeft:右左どっち！
	static NVector2 GetStick(const bool isLeft = true);

	//isVertical:垂直方向か
	//isLstick:Lスティックか
	//上、左はなら-1
	//下、右なら+1が返ってくる
	static uint32_t StickTriggered(bool isVertical, const bool isLstick = true);

	//コントローラーの振動を設定
	//パワーは0.0f~1.0fで入力してね
	static void Vibration(const float leftVibrationPower, const float rightVibrationPower);

	//接続情報取得
	inline static bool GetIsConnect() { return isConnect_; }
};