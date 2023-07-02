#include "LineBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 5.0f;      //弾撃つまでの時間
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

    //リリースでもいじりたいからifdefで囲ってない
    static float coolTimer = 5.0f;
    ImGui::Begin("LineBulletParameter");
    //1F~60Fまでの間にとどめる
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 1.0f, 60.0f);
    ImGui::End();
    shotCoolTimer_.SetMaxTimer(coolTimer);
}
