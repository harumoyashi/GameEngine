#include "NWindows.h"
#include "NFramework.h"
#include "NGame.h"

//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t) {
	//������
	std::unique_ptr<NFramework> game = std::make_unique<NGame>();
	game->Run();
	return 0;
}