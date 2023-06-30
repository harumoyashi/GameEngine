#pragma once
#include "BulletManager.h"

//弾生成工場
//循環参照が起きないようにBulletManagerと分けてる
class BulletFactory
{
private:
	//std::unique_ptr<IBullet> bulletType_;

public:
	static BulletFactory* GetInstance();

	//弾の生成
	void Create(IBullet::BulletType type,NVector3 pos);
};