#pragma once
#include <list>
#include <memory>

#include "IBullet.h"

class BulletManager
{
public:
	std::vector<std::unique_ptr<IBullet>> bullets_;	//’e‚Ç‚à

public:
	static BulletManager* GetInstance();

	void Init();
	void Update();
	void Draw();
};