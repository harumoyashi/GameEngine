#pragma once
#include "IBullet.h"
#include "NTimer.h"

class SideBullet:
	public IBullet
{
private:
    NTimer shotCoolTimer_;		//�e���܂ł̎���
    NTimer shortShotCoolTimer_;	//1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    bool isCanShot_;			//���Ă邩�t���O
    uint32_t level_;            //�e�̋������x��
    uint32_t allAtOnceNum_;     //��x�Ɍ��e�̐�

public:
    SideBullet();
};