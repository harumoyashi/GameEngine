#include "LineBullet.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 0.2f;      //�e���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 1;                 //�e�̋������x��
    allAtOnceNum_ = 1;          //��x�Ɍ��e�̐�
    
    //obj_->color_ = NColor::kRed;  //�Ȃ����G���[�o��
    SetDamage(1.0f);
}
