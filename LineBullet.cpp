#include "LineBullet.h"
#include "Player.h"

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
    SetElapseSpeed(Player::GetInstance()->GetMoveVelo().Length());

    shotCoolTimer_.Update(elapseSpeed_);
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }
}
