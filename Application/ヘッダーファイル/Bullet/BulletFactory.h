#pragma once
#include "BulletManager.h"
#include "LineBullet.h"
#include "SideBullet.h"

//弾生成工場
//循環参照が起きないようにBulletManagerと分けてる
class BulletFactory final
{
private:
	LineBullet line_;	//通常弾
	SideBullet side_;	//サイド弾

public:
	static BulletFactory* GetInstance();

	void Update();
	//弾の生成
	void Create(IBullet::BulletType type,NVec3 pos,uint32_t level);

private:
	BulletFactory() = default;
	BulletFactory(const BulletFactory&) = delete;
	~BulletFactory() = default;
	BulletFactory& operator=(const BulletFactory&) = delete;
};