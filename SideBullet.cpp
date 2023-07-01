#include "SideBullet.h"

SideBullet::SideBullet()
{
    shotCoolTimer_ = 60.0f;		//�e���܂ł̎���
    shortShotCoolTimer_ = 5.0f; //1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 0;                 //�e�̋������x��
    allAtOnceNum_ = 2;          //��x�Ɍ��e�̐�
}

void SideBullet::SideUpdate()
{
    shotCoolTimer_.Update();
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }
}