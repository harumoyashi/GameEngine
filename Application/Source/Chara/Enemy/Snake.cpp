#include "Snake.h"
#include "NCollisionManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "Score.h"

Snake::Snake()
{
	enemyTypeName_ = "snake";	//種族名決める(ホントはモデルネームと統一したい)
}