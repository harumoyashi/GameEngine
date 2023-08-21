#pragma once
#include <memory>

#include "IBullet.h"
#include "NQuaternion.h"

class BulletManager final
{
public:
	std::vector<std::unique_ptr<IBullet>> bullets_;	//’e‚Ç‚à
	NQuaternion q;

public:
	static BulletManager* GetInstance();

	void Init();
	void Update();
	void Draw();

	static NMatrix4 Q(const NQuaternion& q);

private:
	BulletManager() = default;
	BulletManager(const BulletManager&) = delete;
	~BulletManager() = default;
	BulletManager& operator=(const BulletManager&) = delete;
};