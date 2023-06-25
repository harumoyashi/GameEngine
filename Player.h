#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

#include "Bullet.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj;
	SphereCollider collider;	//�v���C���[�̓����蔻��

	std::list<std::unique_ptr<Bullet>> bullets;	//�e�ǂ�
	//NTimer shotTimer;		//�e������
	NTimer shotCoolTimer;	//�e���܂ł̎���

public:
	void Init();
	void Update();
	void Draw();
};