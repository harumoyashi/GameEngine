#include "EnemyFactory.h"
#include "NCollision.h"
#include "NParticleManager.h"

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
		for (uint32_t i = 0; i < wolfSideNum; i++)
		{
			//�Ƃ肠�����ǉ�
			EnemyManager::GetInstance()->enemys_.emplace_back();
			//�Ή�������ނɏ��L���������Đ���
			EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Wolf>();

			//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɓo�^
			EnemyManager::GetInstance()->enemys_.back()->AddEmitter((uint32_t)EnemyManager::GetInstance()->enemys_.size() - 1);

			//�o���ʒu��͈͓����烉���_���Őݒ�
			offset = { MathUtil::Randomf(-wolfSideEmitter.x,wolfSideEmitter.x),
				0,MathUtil::Randomf(-wolfSideEmitter.y,wolfSideEmitter.y) };

			//�z�u�ʒu�����S���E�����Ői�s�����ς���
			if (pos.x > 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(270.0f), wolf.modelname);
			}
			else
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(90.0f), wolf.modelname);
			}
		}

		//�����ʒu���Ȃ��悤�ɂ��鏈��
		do
		{
			isCollision = false;	//while������ƂɃ��Z�b�g

			for (uint32_t i = 0; i < wolfSideNum; i++)
			{
				for (uint32_t j = 0; j < wolfSideNum; j++)
				{
					if (i != j)		//�����I�u�W�F�N�g���m�͔��肵�Ȃ�
					{
						size_t enemySize = EnemyManager::GetInstance()->enemys_.size() - 1;

						//������������瓯�m�œ����蔻����
						bool isCol =
							NCollision::SphereCol(
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetCollider(),
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetCollider());

						//�������Ă��Ȃ�
						if (isCol)
						{
							//�o���ʒu��͈͓����烉���_���Őݒ�
							offset = { MathUtil::Randomf(-wolfSideEmitter.x,wolfSideEmitter.x),
								0,MathUtil::Randomf(-wolfSideEmitter.y,wolfSideEmitter.y) };
							//���W�Đݒ�
							EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->SetPos(pos + offset);

							isCollision = true;	//���ł��������Ă��Ȃ��蒼��
						}
					}
				}
			}
		} while (isCollision == true);

		break;

	default:
		break;
	}
}
