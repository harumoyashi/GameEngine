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
	//寿命が尽きた＋死亡パーティクル出し切った敵を全削除
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetisAlive() == false && enemys_[i]->GetParticlesDead())
		{
			enemys_.erase(enemys_.begin() + i);
			i = (size_t)-1;
			isEnemyDead = true;
		}
	}

	//if (isEnemyDead)	//もし誰か死んだら
	//{
	//	for (size_t i = 0; i < enemys_.size(); i++)
	//	{
	//		//パーティクルエミッターをマネージャーに再登録
	//		EnemyManager::GetInstance()->enemys_[i]->AddEmitter(std::to_string(i));
	//	}
	//}

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
