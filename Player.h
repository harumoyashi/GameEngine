#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

#include "Bullet.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj;
	SphereCollider collider;	//ƒvƒŒƒCƒ„[‚Ì“–‚½‚è”»’è

	std::list<std::unique_ptr<Bullet>> bullets;	//’e‚Ç‚à
	//NTimer shotTimer;		//’eŒ‚‚ÂŠÔ
	NTimer shotCoolTimer;	//’eŒ‚‚Â‚Ü‚Å‚ÌŠÔ

public:
	void Init();
	void Update();
	void Draw();
};