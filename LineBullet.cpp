#include "LineBullet.h"
#include "Player.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 8.0f;      //弾撃つまでの時間
    isCanShot_ = true;			//撃てるかフラグ
    level_ = 1;                 //弾の強化レベル
    
    //obj_->color_ = NColor::kRed;  //なぜかエラー出る
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
