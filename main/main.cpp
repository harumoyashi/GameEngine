#include "NWindows.h"
#include "NGame.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//初期化
	NGame game;
	game.Init();
	//ゲームループ
	while (true)
	{
		game.Update();
		if (game.GetIsGameEnd())
		{
			break;
		}
		game.Draw();
	}
	game.Finalize();
	return 0;
}