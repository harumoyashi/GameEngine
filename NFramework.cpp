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
	//������
	Init();
	//�Q�[�����[�v
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
