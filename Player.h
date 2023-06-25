#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

#include "Bullet.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj;
	SphereCollider collider;	//プレイヤーの当たり判定

	std::list<std::unique_ptr<Bullet>> bullets;	//弾ども
	//NTimer shotTimer;		//弾撃つ時間
	NTimer shotCoolTimer;	//弾撃つまでの時間

public:
	void Init();
	void Update();
	void Draw();
};