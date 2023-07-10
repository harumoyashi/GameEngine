#include "EnemyFactory.h"

EnemyFactory* EnemyFactory::GetInstance()
{
	static EnemyFactory instance;
	return &instance;
}

void EnemyFactory::Update()
{
	//wolf.Update();
}

void EnemyFactory::Create(IEnemy::EnemyType type, NVector3 pos)
{
	switch (type)
	{
	case IEnemy::EnemyType::WolfSide:
		//とりあえず追加
		EnemyManager::GetInstance()->enemys_.emplace_back();
		//対応した種類に所有権持たせて生成
		EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Wolf>();
		//配置位置が中心より右か左で進行方向変える
		if (pos.x > 0)
		{
			EnemyManager::GetInstance()->enemys_.back()->Generate(
				pos, MathUtil::Degree2Radian(270.0f), wolf.modelname);
		}
		else
		{
			EnemyManager::GetInstance()->enemys_.back()->Generate(
				pos, MathUtil::Degree2Radian(90.0f), wolf.modelname);
		}

		break;

	default:
		break;
	}
}
