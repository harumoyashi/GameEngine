#include "BulletFactory.h"
#include "NAudioManager.h"

BulletFactory* BulletFactory::GetInstance()
{
	static BulletFactory instance;
	return &instance;
}

void BulletFactory::Update()
{
	line_.LineUpdate();
	side_.SideUpdate();
}

void BulletFactory::Create(IBullet::BulletType type, NVector3 pos, uint32_t level)
{
	switch (type)
	{
	case IBullet::BulletType::LineBullet:
		if (line_.GetIsCanShot())	//撃てるなら
		{
			//とりあえず追加
			BulletManager::GetInstance()->bullets_.emplace_back();
			//レベル格納して(レベル用の処理未対応)
			line_.SetLevel(level);
			//対応した種類に所有権持たせて生成
			BulletManager::GetInstance()->bullets_.back() = std::make_unique<LineBullet>();
			BulletManager::GetInstance()->bullets_.back()->Generate(pos);
			//生成が終わったらタイマーとフラグをリセット
			line_.SetIsCanShot(false);
			line_.ReSetShotCoolTimer();

			NAudioManager::Play("shotSE",false,0.5f);
		}

		break;
	case IBullet::BulletType::SideBullet:
		if (side_.GetIsCanShot())	//撃てるなら
		{
			//2発分回す(後でレベルに対応したfor文さらに回す)
			for (size_t i = 0; i < side_.GetAllAtOnceNum(); i++)
			{
				//とりあえず追加
				BulletManager::GetInstance()->bullets_.emplace_back();
				//レベル格納して(レベル用の処理未対応)
				side_.SetLevel(level);
				//対応した種類に所有権持たせて生成
				BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
				//左右に出すように
				BulletManager::GetInstance()->bullets_.back()->Generate(pos, MathUtil::Degree2Radian(90.0f + 180.0f * i));
			}
			//生成が終わったらタイマーとフラグをリセット
			side_.SetIsCanShot(false);
			side_.ReSetShotCoolTimer();
		}

		break;
	default:
		break;
	}
}