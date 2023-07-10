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
		//�Ƃ肠�����ǉ�
		EnemyManager::GetInstance()->enemys_.emplace_back();
		//�Ή�������ނɏ��L���������Đ���
		EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Wolf>();
		//�z�u�ʒu�����S���E�����Ői�s�����ς���
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
