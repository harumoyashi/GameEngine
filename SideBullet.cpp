#include "SideBullet.h"

SideBullet::SideBullet()
{
    shotCoolTimer_ = 0.8f;		//�e���܂ł̎���
    shortShotCoolTimer_ = 0.1f; //1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 0;                 //�e�̋������x��
    allAtOnceNum_ = 2;          //��x�Ɍ��e�̐�
}
