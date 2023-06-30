#include "BulletFactory.h"
#include "LineBullet.h"
#include "SideBullet.h"

BulletFactory* BulletFactory::GetInstance()
{
	static BulletFactory instance;
	return &instance;
}

void BulletFactory::Create(IBullet::BulletType type, NVector3 pos)
{
	BulletManager::GetInstance()->bullets_.emplace_back();
	switch (type)
	{
	case IBullet::BulletType::LineBullet:
		BulletManager::GetInstance()->bullets_.back() = std::make_unique<LineBullet>();
		BulletManager::GetInstance()->bullets_.back()->Generate(pos);

		break;
	case IBullet::BulletType::SideBullet:
		BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
		BulletManager::GetInstance()->bullets_.back()->Generate(pos);

		break;
	default:
		break;
	}
}