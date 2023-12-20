#pragma once
#include "IEnemy.h"
#include "CircleCollider.h"

class Mouse final :
    public IEnemy
{
public:
    std::string modelname = "mouse";    //モデル名
    CircleCollider collider_;		    //コライダー

    Mouse();

    // ゲッター //
    //コライダー取得
    const CircleCollider GetCollider()const { return collider_; }
};