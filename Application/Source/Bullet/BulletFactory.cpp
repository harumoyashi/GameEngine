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
	wide_.WideUpdate();
}

void BulletFactory::Create(BulletType type, NVec3 pos, uint32_t level)
{
	switch (type)
	{
	case BulletType::LineBullet:
		if (line_.GetIsCanShot())	//撃てるなら
		{
			//レベル格納して(レベル用の処理未対応)
			line_.SetLevel(level);
			for (uint32_t i = 0; i < line_.GetLevel(); i++)
			{
				//レベルに応じて追加
				if (BulletManager::GetInstance()->GetIsEmpty())	//満杯なら最初に追加した奴消す
				{
					BulletManager::GetInstance()->EraceBegin();
				}
				BulletManager::GetInstance()->bullets_.emplace_back();

				//対応した種類に所有権持たせて生成
				BulletManager::GetInstance()->bullets_.back() = std::make_unique<LineBullet>();

				//奇数か偶数かで左右に振り分け親方
				if (i % 2 == 0)
				{
					//基準となる座標から横にずらして生成
					NVec3 generatePos = pos;
					generatePos.x += (float)(i / 2) * 0.3f;
					BulletManager::GetInstance()->bullets_.back()->Generate(generatePos);
				}
				else
				{
					//1足してあげないと2の時0で重なっちゃう
					NVec3 generatePos = pos;
					generatePos.x -= (float)(i / 2 + 1) * 0.3f;
					BulletManager::GetInstance()->bullets_.back()->Generate(generatePos);
				}
			}
			//生成が終わったらタイマーとフラグをリセット
			line_.SetIsCanShot(false);
			line_.ReSetShotCoolTimer();

			NAudioManager::GetInstance()->Play("shotSE", false, 0.4f);
		}

		break;
	case BulletType::SideBullet:
		if (side_.GetIsCanShot())	//撃てるなら
		{
			//撃った回数カウント用
			static uint32_t shotCount;
			//レベルに応じて追加
			if (side_.GetIsShortCanShot())	//撃てるなら
			{
				//2発分回す(後でレベルに対応したfor文さらに回す)
				for (size_t j = 0; j < side_.GetAllAtOnceNum(); j++)
				{
					//とりあえず追加
					if (BulletManager::GetInstance()->GetIsEmpty())	//満杯なら最初に追加した奴消す
					{
						BulletManager::GetInstance()->EraceBegin();
					}
					BulletManager::GetInstance()->bullets_.emplace_back();

					//レベル格納して(レベル用の処理未対応)
					side_.SetLevel(level);
					//対応した種類に所有権持たせて生成
					BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
					//左右に出すように
					BulletManager::GetInstance()->bullets_.back()->Generate(pos, MathUtil::Degree2Radian(90.0f + 180.0f * j));
				}
				//生成が終わったらタイマーとフラグをリセット
				side_.SetIsShortCanShot(false);
				side_.ReSetShortShotCoolTimer();
				shotCount++;	//撃った回数を加算してく

				NAudioManager::GetInstance()->Play("shotSE", false, 0.4f);
			}

			//撃った回数が弾レベルに達したら終わり
			if (shotCount >= side_.GetLevel())
			{
				shotCount = 0;
				//生成が終わったらタイマーとフラグをリセット
				side_.SetIsCanShot(false);
				side_.ReSetShotCoolTimer();
			}
		}

		break;

	case BulletType::WideBullet:
		if (wide_.GetIsCanShot())	//撃てるなら
		{
			//撃った回数カウント用
			static uint32_t shotCount;
			//レベルに応じて追加
			if (wide_.GetIsShortCanShot())	//撃てるなら
			{
				//2発分回す(後でレベルに対応したfor文さらに回す)
				for (size_t j = 0; j < wide_.GetAllAtOnceNum(); j++)
				{
					//とりあえず追加
					if (BulletManager::GetInstance()->GetIsEmpty())	//満杯なら最初に追加した奴消す
					{
						BulletManager::GetInstance()->EraceBegin();
					}
					BulletManager::GetInstance()->bullets_.emplace_back();

					//レベル格納して(レベル用の処理未対応)
					wide_.SetLevel(level);
					//対応した種類に所有権持たせて生成
					BulletManager::GetInstance()->bullets_.back() = std::make_unique<SideBullet>();
					//左右に出すように
					BulletManager::GetInstance()->bullets_.back()->Generate(pos, MathUtil::Degree2Radian(-45.0f + 90.0f * j));
				}
				//生成が終わったらタイマーとフラグをリセット
				wide_.SetIsShortCanShot(false);
				wide_.ReSetShortShotCoolTimer();
				shotCount++;	//撃った回数を加算してく

				NAudioManager::GetInstance()->Play("shotSE", false, 0.4f);
			}

			//撃った回数が弾レベルに達したら終わり
			if (shotCount >= wide_.GetLevel())
			{
				shotCount = 0;
				//生成が終わったらタイマーとフラグをリセット
				wide_.SetIsCanShot(false);
				wide_.ReSetShotCoolTimer();
			}
		}

		break;
	default:
		break;
	}
}