#include "NWindows.h"
#include "NFramework.h"
#include "NGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//初期化
	std::unique_ptr<NFramework> game = std::make_unique<NGame>();
	game->Run();
	return 0;
}