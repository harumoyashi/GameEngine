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
			//とりあえず追加
			EnemyManager::GetInstance()->enemys_.emplace_back();
			//対応した種類に所有権持たせて生成
			EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Wolf>();

			//パーティクルエミッターをマネージャーに登録
			EnemyManager::GetInstance()->enemys_.back()->AddEmitter((uint32_t)EnemyManager::GetInstance()->enemys_.size() - 1);

			NVector3 offset = { MathUtil::Randomf(-2.0f,2.0f),0,MathUtil::Randomf(-1.0f,1.0f) };
			//配置位置が中心より右か左で進行方向変える
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

		//生成位置被らないようにする処理
		do
		{
			isCollision = false;	//while一周ごとにリセット

			for (uint32_t i = 0; i < wolfSideNum; i++)
			{
				for (uint32_t j = 0; j < wolfSideNum; j++)
				{
					if (i != j)		//同じオブジェクト同士は判定しない
					{
						size_t enemySize = EnemyManager::GetInstance()->enemys_.size() - 1;

						//今生成したやつら同士で当たり判定取る
						bool isCol =
							NCollision::SphereCol(
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetCollider(),
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetCollider());

						//当たってたなら
						if (isCol)
						{
							NVector3 posI = EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetPos();
							NVector3 posJ = EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetPos();
							//被らなくなるまで押し出し
							NVector3 vec = posI - posJ;

							//最後ちょっと大きくしとかないと少数の差が小さすぎてオーバーフロー起こる
							vec = vec.Normalize() * EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetScale() * 5.0f;
							EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->SetPos(pos + vec);

							posI = EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetPos();
							posJ = EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetPos();

							isCollision = true;	//一回でも当たってたならやり直し
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
