#include "LineBullet.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 0.2f;      //弾撃つまでの時間
    isCanShot_ = true;			//撃てるかフラグ
    level_ = 1;                 //弾の強化レベル
    allAtOnceNum_ = 1;          //一度に撃つ弾の数
    
    //obj_->color_ = NColor::kRed;  //なぜかエラー出る
    SetDamage(1.0f);
}
