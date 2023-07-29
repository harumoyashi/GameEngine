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
	//õ–½‚ªs‚«‚½{€–Sƒp[ƒeƒBƒNƒ‹o‚µØ‚Á‚½“G‚ğ‘Síœ
	for (size_t i = 0; i < enemys_.size(); i++)
	{
		if (enemys_[i]->GetisAlive() == false && enemys_[i]->GetParticlesDead())
		{
			enemys_.erase(enemys_.begin() + i);
			//ƒGƒ~ƒbƒ^[ŒQ‚©‚çíœ
			NParticleManager::GetInstance()->enemyEmitters_.erase(
				NParticleManager::GetInstance()->enemyEmitters_.begin() + i);
			i = (size_t)-1;
			isEnemyDead = true;
		}
	}

	if (isEnemyDead)	//‚à‚µ’N‚©€‚ñ‚¾‚ç
	{
		for (uint32_t i = 0; i < enemys_.size(); i++)
		{
			//“G‚Ì¯•Ê”Ô†‚ğÄ“o˜^
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