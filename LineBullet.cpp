#include "LineBullet.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 8.0f;      //�e���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 1;                 //�e�̋������x��
    
    //obj_->color_ = NColor::kRed;  //�Ȃ����G���[�o��
    SetDamage(1.0f);
}

void LineBullet::LineUpdate()
{
    shotCoolTimer_.Update();
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }
}
