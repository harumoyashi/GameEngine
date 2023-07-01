#include "BulletFactory.h"

BulletFactory* BulletFactory::GetInstance()
{
	static BulletFactory instance;
	return &instance;
}

void BulletFactory::Update()
{
	line_.LineUpdate();
	//side_.Update();
}

void BulletFactory::Create(IBullet::BulletType type, NVector3 pos, uint32_t level)
{
	switch (type)
	{
	case IBullet::BulletType::LineBullet:
		if (line_.GetIsCanShot())
		{
			BulletManager::GetInstance()->bullets_.emplace_back();
			line_.SetLevel(level);
			BulletManager::GetInstance()->bullets_.back() = std::make_unique<LineBullet>();
			BulletManager::GetInstance()->bullets_.back()->Generate(pos);
			line_.SetIsCanShot(false);
			line_.ReSetShotCoolTimer();
		}

		break;
	case IBullet::BulletType::SideBullet:
		BulletManager::GetInstance()->bullets_.emplace_back();
		BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
		BulletManager::GetInstance()->bullets_.back()->Generate(pos);

		break;
	default:
		break;
	}
}