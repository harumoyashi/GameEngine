#pragma once
#include "IBullet.h"
#include "NTimer.h"

//�ʏ�̒e
//�����Ŗ{����������
class LineBullet:
    public IBullet
{
private:
    NTimer shotCoolTimer_;		//�e���܂ł̎���
    bool isCanShot_;			//���Ă邩�t���O
    uint32_t level_;            //�e�̋������x��
    uint32_t allAtOnceNum_;     //��x�Ɍ��e�̐�

public:
    LineBullet();
};

