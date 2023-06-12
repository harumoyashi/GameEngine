#pragma once
#include <string>
#include <windows.h>

class NWindows
{
public:
	static const uint32_t sWin_width = 1280;
	static const uint32_t sWin_height = 720;

private:
	//ウィンドウクラスの生成＆登録
	WNDCLASSEX win_{};
	//ウィンドウハンドルの生成
	HWND hwnd_;
	//ウィンドウサイズ用の長方形の生成
	RECT wrc_;
	//メッセージ情報構造体の生成
	MSG msg_ = {};

public:
#pragma region 初期化
	//シングルトンインスタンス取得
	static NWindows* GetInstance();

	//ウィンドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	//Windowクラスの設定
	void Set();
	//コンソールへの文字出力
	void DebugText(LPCSTR text);
	//ウィンドウオブジェクトの生成
	void CreateWindowObj();
	//ウィンドウ表示
	void Display();
#pragma endregion
#pragma region 更新
	//プロシージャにメッセージを送る
	bool WindowMessage();
#pragma endregion
#pragma region 終了
	//終了処理
	//クラスの解除
	void Finalize();
#pragma endregion
#pragma region ゲッター
	inline HWND GetHwnd()const { return hwnd_; }
	inline HINSTANCE GetHInstance()const { return win_.hInstance; }
#pragma endregion
};