#include "Snake.h"

Snake::Snake()
{
	enemyTypeName_ = "snake";	//種族名決める(ホントはモデルネームと統一したい)

	//パラメーター設定
	hp_ = 3;
	score_ = 20;
}