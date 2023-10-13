#include "NWindows.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"winmm.lib")

//ここ変えると怒られるので先生に聞く
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam);

NWindows* NWindows::GetInstance()
{
	static NWindows instance;
	return &instance;
}

//ウィンドプロシージャ
//面倒だけど書かなきゃいけない関数
LRESULT NWindows::WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui用ウィンドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	switch (msg)
	{
		//ウィンドウが破棄されたら呼ばれる
	case WM_DESTROY:
		PostQuitMessage(0);	//OSに対して「もうこのアプリは終わる」と伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);	//既定の処理を行う
}

//Windowクラスの設定
void NWindows::Set()
{
	win_.cbSize = sizeof(WNDCLASSEX);
	win_.lpfnWndProc = (WNDPROC)WindowProc;		//ウィンドウプロシージャを設定
	win_.lpszClassName = L"DX12Sample";			//アプリケーションクラス名
	win_.hInstance = GetModuleHandle(nullptr);		//ハンドルの取得
	win_.hCursor = LoadCursor(NULL, IDC_ARROW);	//カーソル指定

	RegisterClassEx(&win_);	//アプリケーションクラス（ウィンドウクラスの指定をOSに伝える）

	wrc_ = { 0,0,kWin_width,kWin_height };	//ウィンドウサイズを決める

	//関数を使ってウィンドウのサイズを補正する
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	//システムタイマーの分解度を上げる
	timeBeginPeriod(1);
}

//コンソールへの文字出力
void NWindows::DebugText(const std::string& text)
{
	OutputDebugStringA(text.c_str());
}

//ウィンドウオブジェクトの生成
void NWindows::CreateWindowObj()
{
	//ウィンドウオブジェクトの生成
	hwnd_ = CreateWindow(
		win_.lpszClassName,		//クラス名指定
		L"DX12テスト",			//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	//タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			//表示x座標はOSにお任せ
		CW_USEDEFAULT,			//表示y座標はOSにお任せ
		wrc_.right - wrc_.left,	//ウィンドウ幅
		wrc_.bottom - wrc_.top,	//ウィンドウ高
		nullptr,				//親ウィンドウハンドル
		nullptr,				//メニューハンドル
		win_.hInstance,			//呼び出しアプリケーションハンドル
		nullptr					//追加パラメーター
	);
}

//ウィンドウ表示
void NWindows::Display()
{
	ShowWindow(hwnd_, SW_SHOW);
}

bool NWindows::WindowMessage()
{
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg_);	//キー入力メッセージの処理
		DispatchMessage(&msg_);	//プロシージャにメッセージを送る
	}

	//アプリケーションが終わるときにmwssageがWM_QUITになる
	if (msg_.message == WM_QUIT)
	{
		return true;	//終了メッセージが来たらループを抜ける
	}

	return false;
}

void NWindows::Finalize()
{
	//もうクラスは使わないので登録解除する
	UnregisterClass(win_.lpszClassName, win_.hInstance);
}