#pragma once
#include "BulletManager.h"
#include "LineBullet.h"
#include "SideBullet.h"

//�e�����H��
//�z�Q�Ƃ��N���Ȃ��悤��BulletManager�ƕ����Ă�
class BulletFactory
{
private:
	LineBullet line_;
	SideBullet side_;

public:
	static BulletFactory* GetInstance();

	void Update();
	//�e�̐���
	void Create(IBullet::BulletType type,NVector3 pos,uint32_t level);
};