#include "NFramework.h"

void NFramework::Init()
{
}

void NFramework::Update()
{
}

void NFramework::Finalize()
{
}

void NFramework::Run()
{
	//初期化
	Init();
	//ゲームループ
	while (true)
	{
		Update();
		if (GetIsGameEnd())
		{
			break;
		}
		Draw();
	}
	Finalize();
}
