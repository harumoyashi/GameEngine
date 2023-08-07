#pragma once
#include "BulletManager.h"
#include "LineBullet.h"
#include "SideBullet.h"

//’e¶¬Hê
//zŠÂQÆ‚ª‹N‚«‚È‚¢‚æ‚¤‚ÉBulletManager‚Æ•ª‚¯‚Ä‚é
class BulletFactory final
{
private:
	LineBullet line_;	//’Êí’e
	SideBullet side_;	//ƒTƒCƒh’e

public:
	static BulletFactory* GetInstance();

	void Update();
	//’e‚Ì¶¬
	void Create(IBullet::BulletType type,NVec3 pos,uint32_t level);

private:
	BulletFactory() = default;
	BulletFactory(const BulletFactory&) = delete;
	~BulletFactory() = default;
	BulletFactory& operator=(const BulletFactory&) = delete;
};