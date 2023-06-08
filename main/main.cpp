#include "NWindows.h"
#include "NFramework.h"
#include "NGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {
	//初期化
	std::unique_ptr<NFramework> game = std::make_unique<NGame>();
	game->Run();
	return 0;
}