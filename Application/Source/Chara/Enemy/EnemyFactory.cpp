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
}

void EnemyFactory::Create(IEnemy::EnemyType type, NVec3 pos, bool isItem)
{
	switch (type)
	{
	case IEnemy::EnemyType::MouseSide:
		for (uint32_t i = 0; i < mouseSideNum; i++)
		{
			//とりあえず追加
			EnemyManager::GetInstance()->enemys_.emplace_back();
			//対応した種類に所有権持たせて生成
			EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Mouse>();

			//パーティクルエミッターをマネージャーに登録
			EnemyManager::GetInstance()->enemys_.back()->AddEmitter((uint32_t)EnemyManager::GetInstance()->enemys_.size() - 1);

			//出現位置を範囲内からランダムで設定
			offset = { MathUtil::Randomf(-mouseSideEmitter.x,mouseSideEmitter.x),
				0,MathUtil::Randomf(-mouseSideEmitter.y,mouseSideEmitter.y) };

			//配置位置が中心より右か左で進行方向変える
			if (pos.x > 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(270.0f), mouse.modelname);
			}
			else
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(90.0f), mouse.modelname);
			}

			//アイテム持った敵作る場合、最初の一体にだけ持たせる
			if (isItem && i == 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->SetisItem(true);
			}
		}

		//生成位置被らないようにする処理
		do
		{
			isCollision = false;	//while一周ごとにリセット

			for (uint32_t i = 0; i < mouseSideNum; i++)
			{
				for (uint32_t j = 0; j < mouseSideNum; j++)
				{
					if (i != j)		//同じオブジェクト同士は判定しない
					{
						size_t enemySize = EnemyManager::GetInstance()->enemys_.size() - 1;

						//今生成したやつら同士で当たり判定取る
						bool isCol =
							NCollision::CircleCol(
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetCollider(),
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetCollider());

						//当たってたなら
						if (isCol)
						{
							//出現位置を範囲内からランダムで設定
							offset = { MathUtil::Randomf(-mouseSideEmitter.x,mouseSideEmitter.x),
								0,MathUtil::Randomf(-mouseSideEmitter.y,mouseSideEmitter.y) };
							//座標再設定
							EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->SetPos(pos + offset);

							isCollision = true;	//一回でも当たってたならやり直し
						}
					}
				}
			}
		} while (isCollision == true);

		break;

	case IEnemy::EnemyType::MouseFront:
		for (uint32_t i = 0; i < mouseFrontNum; i++)
		{
			//とりあえず追加
			EnemyManager::GetInstance()->enemys_.emplace_back();
			//対応した種類に所有権持たせて生成
			EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Mouse>();

			//パーティクルエミッターをマネージャーに登録
			EnemyManager::GetInstance()->enemys_.back()->AddEmitter((uint32_t)EnemyManager::GetInstance()->enemys_.size() - 1);

			//出現位置を範囲内からランダムで設定
			offset = { MathUtil::Randomf(-mouseFrontEmitter.x,mouseFrontEmitter.x),
				0,MathUtil::Randomf(-mouseFrontEmitter.y,mouseFrontEmitter.y) };

			//進行方向下に
			EnemyManager::GetInstance()->enemys_.back()->Generate(
				pos + offset, MathUtil::Degree2Radian(180.0f), mouse.modelname);

			//アイテム持った敵作る場合、最初の一体にだけ持たせる
			if (isItem && i == 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->SetisItem(true);
			}
		}

		//生成位置被らないようにする処理
		do
		{
			isCollision = false;	//while一周ごとにリセット

			for (uint32_t i = 0; i < mouseSideNum; i++)
			{
				for (uint32_t j = 0; j < mouseSideNum; j++)
				{
					if (i != j)		//同じオブジェクト同士は判定しない
					{
						size_t enemySize = EnemyManager::GetInstance()->enemys_.size() - 1;

						//今生成したやつら同士で当たり判定取る
						bool isCol =
							NCollision::CircleCol(
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)i)->GetCollider(),
								EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->GetCollider());

						//当たってたなら
						if (isCol)
						{
							//出現位置を範囲内からランダムで設定
							offset = { MathUtil::Randomf(-mouseSideEmitter.x,mouseSideEmitter.x),
								0,MathUtil::Randomf(-mouseSideEmitter.y,mouseSideEmitter.y) };
							//座標再設定
							EnemyManager::GetInstance()->enemys_.at(enemySize - (size_t)j)->SetPos(pos + offset);

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
