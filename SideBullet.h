#pragma once
#include "IBullet.h"
#include "NTimer.h"

class SideBullet:
	public IBullet
{
private:
    NTimer shotCoolTimer_;		//弾撃つまでの時間
    NTimer shortShotCoolTimer_;	//1ループで二回以上打つ時に弾撃つまでの時間
    bool isCanShot_;			//撃てるかフラグ
    uint32_t level_;            //弾の強化レベル
    uint32_t allAtOnceNum_;     //一度に撃つ弾の数

public:
    SideBullet();
};