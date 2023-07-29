#include "EnemyManager.h"
#include "NParticleManager.h"
#include "Wave.h"

#include "NImGuiManager.h"
#include "imgui.h"

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
			//�G�~�b�^�[�Q����폜
			NParticleManager::GetInstance()->enemyEmitters_.erase(
				NParticleManager::GetInstance()->enemyEmitters_.begin() + i);
			i = (size_t)-1;
			isEnemyDead = true;
		}
	}

	if (isEnemyDead)	//�����N�����񂾂�
	{
		for (uint32_t i = 0; i < enemys_.size(); i++)
		{
			//�G�̎��ʔԍ����ēo�^
			EnemyManager::GetInstance()->enemys_[i]->SetEnemyNum(i);
		}
		isEnemyDead = false;
	}

	for (auto& enemy : enemys_)
	{
		enemy->Update();
	}

#ifdef DEBUG
	ImGui::Begin("Enemys");
	ImGui::Text("EnemysSize:%d", enemys_.size());
	ImGui::End();
#endif //DEBUG
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}