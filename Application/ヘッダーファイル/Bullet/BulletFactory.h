#pragma once
#include "BulletManager.h"
#include "LineBullet.h"
#include "SideBullet.h"

//�e�����H��
//�z�Q�Ƃ��N���Ȃ��悤��BulletManager�ƕ����Ă�
class BulletFactory final
{
private:
	LineBullet line_;	//�ʏ�e
	SideBullet side_;	//�T�C�h�e

public:
	static BulletFactory* GetInstance();

	void Update();
	//�e�̐���
	void Create(IBullet::BulletType type,NVec3 pos,uint32_t level);

private:
	BulletFactory() = default;
	BulletFactory(const BulletFactory&) = delete;
	~BulletFactory() = default;
	BulletFactory& operator=(const BulletFactory&) = delete;
};