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
	//寿命が尽きた＋死亡パーティクル出し切った敵を全削除
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetisAlive() == false && enemys_[i]->GetParticlesDead())
		{
			enemys_.erase(enemys_.begin() + i);
			//エミッター群から削除
			NParticleManager::GetInstance()->enemyEmitters_.erase(
				NParticleManager::GetInstance()->enemyEmitters_.begin() + i);
			i = (size_t)-1;
			isEnemyDead = true;
		}
	}

	if (isEnemyDead)	//もし誰か死んだら
	{
		for (uint32_t i = 0; i < enemys_.size(); i++)
		{
			//敵の識別番号を再登録
			EnemyManager::GetInstance()->enemys_[i]->SetEnemyNum(i);
		}
		isEnemyDead = false;
	}

	for (auto& enemy : enemys_)
	{
		enemy->Update();
	}

	//リリースでも見たいからifdefで囲ってない
	ImGui::Begin("Enemys");
	ImGui::Text("EnemysSize:%d", enemys_.size());
	ImGui::End();
}

void EnemyManager::Draw()
{
	for (auto& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyManager::DrawSprite()
{
	for (auto& enemy : enemys_)
	{
		enemy->DrawSprite();
	}
}
