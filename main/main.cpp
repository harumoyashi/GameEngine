#include "NWindows.h"
#include "NGame.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//������
	NGame game;
	game.Init();
	//�Q�[�����[�v
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