#pragma once
#include <memory>

#include "IBullet.h"

class BulletManager final
{
public:
	std::vector<std::unique_ptr<IBullet>> bullets_;	//弾ども

public:
	static BulletManager* GetInstance();

	void Init();
	void Update();
	void Draw();

private:
	BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	~BulletManager() = default;
	BulletManager& operator=(const BulletManager&) = delete;
};