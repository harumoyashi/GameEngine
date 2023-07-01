#include "SideBullet.h"

SideBullet::SideBullet()
{
    shotCoolTimer_ = 60.0f;		//弾撃つまでの時間
    shortShotCoolTimer_ = 5.0f; //1ループで二回以上打つ時に弾撃つまでの時間
    isCanShot_ = true;			//撃てるかフラグ
    level_ = 0;                 //弾の強化レベル
    allAtOnceNum_ = 2;          //一度に撃つ弾の数
}

void SideBullet::SideUpdate()
{
    shotCoolTimer_.Update();
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }
}