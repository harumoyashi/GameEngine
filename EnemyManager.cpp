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
	//�������s�����{���S�p�[�e�B�N���o���؂����G��S�폜
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetisAlive() == false && enemys_[i]->GetParticlesDead())
		{
			enemys_.erase(enemys_.begin() + i);
			i = (size_t)-1;
			isEnemyDead = true;
		}
	}

	//if (isEnemyDead)	//�����N�����񂾂�
	//{
	//	for (size_t i = 0; i < enemys_.size(); i++)
	//	{
	//		//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɍēo�^
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
