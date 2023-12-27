#pragma once
#include <memory>
#include "IBullet.h"

// 弾管理 //

class BulletManager final
{
public:
	std::vector<std::unique_ptr<IBullet>> bullets_;	//弾ども
	const uint32_t maxBul = 126;	//最大弾数

public:
	static BulletManager* GetInstance();

	void Init();
	void Update();
	void Draw();

	//最初の要素削除
	void EraceBegin() { bullets_.erase(bullets_.begin()); }

	//満杯かどうか返す
	bool GetIsEmpty() { return bullets_.size() >= maxBul; }

private:
	BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	~BulletManager() = default;
	BulletManager& operator=(const BulletManager&) = delete;
};