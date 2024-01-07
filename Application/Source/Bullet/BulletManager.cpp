#include "BulletManager.h"
#include "NParticleManager.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

BulletManager* BulletManager::GetInstance()
{
	static BulletManager instance;
	return &instance;
}

void BulletManager::Init()
{
	bullets_.clear();
}

void BulletManager::Update()
{
	//寿命が尽きた弾を全削除
	for (uint32_t i = 0; i < bullets_.size(); i++)
	{
		if (bullets_[i]->GetisAlive() == false && bullets_[i]->GetParticlesDead())
		{
			bullets_.erase(bullets_.begin() + i);
			//エミッター群から削除
			NParticleManager::GetInstance()->bulletEmitters_.erase(
				NParticleManager::GetInstance()->bulletEmitters_.begin() + i);
			i--;

			isBulletDead = true;
		}
	}

	if (isBulletDead)	//もし誰か死んだら
	{
		for (uint32_t i = 0; i < bullets_.size(); i++)
		{
			//敵の識別番号を再登録
			BulletManager::GetInstance()->bullets_[i]->SetBulletNum(i);
		}
		isBulletDead = false;
	}

	for (auto& bullet : bullets_)
	{
		bullet->Update();
	}


#ifdef _DEBUG
	float bulNum = (float)bullets_.size();
	ImGui::Begin("Bullets");
	ImGui::Text("num:%f", bulNum);
	ImGui::Text("elapse:%f", Player::GetInstance()->GetElapseSpeed());
	ImGui::End();
#endif
}

void BulletManager::Draw()
{
	for (auto& bullet : bullets_)
	{
		bullet->Draw();
	}
}

void BulletManager::EraceBegin()
{
	bullets_.erase(bullets_.begin());
	//エミッター群から削除
	NParticleManager::GetInstance()->bulletEmitters_.erase(
		NParticleManager::GetInstance()->bulletEmitters_.begin());
}
