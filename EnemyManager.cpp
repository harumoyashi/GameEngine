#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance()
{
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Init()
{
	enemys_.clear();
}

void EnemyManager::Update()
{
	//�������s�����T��S�폜
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetisAlive() == false)
		{
			enemys_.erase(enemys_.begin() + i);
			i = (size_t)-1;
		}
	}

	for (auto& enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}
