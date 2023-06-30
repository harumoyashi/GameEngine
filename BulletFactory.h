#pragma once
#include "BulletManager.h"

//’e¶¬Hê
//zŠÂQÆ‚ª‹N‚«‚È‚¢‚æ‚¤‚ÉBulletManager‚Æ•ª‚¯‚Ä‚é
class BulletFactory
{
private:
	//std::unique_ptr<IBullet> bulletType_;

public:
	static BulletFactory* GetInstance();

	//’e‚Ì¶¬
	void Create(IBullet::BulletType type,NVector3 pos);
};