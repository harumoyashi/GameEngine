#include "EnemyManager.h"
#include "NParticleManager.h"
#include "Wave.h"
#include "Player.h"

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

	beatInTimer_.Reset();
	beatOutTimer_.Reset();
	beatInTimer_.Start();
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

	//���Y����点�Ƃ�
	float size = 0.f;
	if (beatInTimer_.GetRun())
	{
		beatInTimer_.Update(Player::GetInstance()->GetElapseSpeed());
		size = NEasing::OutQuad(beatInTimer_.GetTimeRate());

		if (beatInTimer_.GetEnd())
		{
			beatOutTimer_.Start();
		}
	}
	else if (beatOutTimer_.GetRun())
	{
		beatOutTimer_.Update(Player::GetInstance()->GetElapseSpeed());
		size = 1.f - NEasing::InBack(beatOutTimer_.GetTimeRate());

		if (beatOutTimer_.GetEnd())
		{
			beatInTimer_.Start();
		}
	}

	//�������̃X�P�[����K�p
	for (auto& enemy : enemys_)
	{
		enemy->SetAddScale(size * Player::GetInstance()->GetScale().x);
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

#ifdef _DEBUG
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