#pragma once
#include "IEnemy.h"
#include "CircleCollider.h"

// ねずみの敵 //

class Mouse final :
    public IEnemy
{
public:
    std::string modelname = "mouse";    //モデル名
    CircleCollider collider_;		    //コライダー

    //コンストラクタ
    Mouse();

    // ゲッター //
    //コライダー取得
    const CircleCollider GetCollider()const { return collider_; }
};