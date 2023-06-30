#include "BulletManager.h"

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
	//寿命が尽きたパーティクルを全削除
	for (size_t i = 0; i < bullets_.size(); i++)
	{
		if (bullets_[i]->GetisAlive() == false)
		{
			bullets_.erase(bullets_.begin() + i);
			i = (size_t)-1;
		}
	}

	for (auto& bullet : bullets_)
	{
		bullet->Update();
	}
}

void BulletManager::Draw()
{
	for (auto& bullet : bullets_)
	{
		bullet->Draw();
	}
}
