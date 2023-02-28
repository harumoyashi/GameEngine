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
	//‰Šú‰»
	Init();
	//ƒQ[ƒ€ƒ‹[ƒv
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
