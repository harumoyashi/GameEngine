#pragma once
#include "BulletManager.h"

//�e�����H��
//�z�Q�Ƃ��N���Ȃ��悤��BulletManager�ƕ����Ă�
class BulletFactory
{
private:
	//std::unique_ptr<IBullet> bulletType_;

public:
	static BulletFactory* GetInstance();

	//�e�̐���
	void Create(IBullet::BulletType type,NVector3 pos);
};