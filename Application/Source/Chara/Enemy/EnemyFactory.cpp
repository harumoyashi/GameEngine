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

		break;

	case IEnemy::EnemyType::SnakeSide:
		for (uint32_t i = 0; i < snakeSideNum; i++)
		{
			//とりあえず追加
			EnemyManager::GetInstance()->enemys_.emplace_back();
			//対応した種類に所有権持たせて生成
			EnemyManager::GetInstance()->enemys_.back() = std::make_unique<Mouse>();

			//パーティクルエミッターをマネージャーに登録
			EnemyManager::GetInstance()->enemys_.back()->AddEmitter((uint32_t)EnemyManager::GetInstance()->enemys_.size() - 1);

			//出現位置を範囲内からランダムで設定
			offset = { MathUtil::Randomf(-snakeSideEmitter.x,snakeSideEmitter.x),
				0,MathUtil::Randomf(-snakeSideEmitter.y,snakeSideEmitter.y) };

			//配置位置が中心より右か左で進行方向変える
			if (pos.x > 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(270.0f), snake.modelname);
			}
			else
			{
				EnemyManager::GetInstance()->enemys_.back()->Generate(
					pos + offset, MathUtil::Degree2Radian(90.0f), snake.modelname);
			}

			//アイテム持った敵作る場合、最初の一体にだけ持たせる
			if (isItem && i == 0)
			{
				EnemyManager::GetInstance()->enemys_.back()->SetisItem(true);
			}
		}

		break;

	default:
		break;
	}
}
