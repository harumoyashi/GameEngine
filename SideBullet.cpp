#include "SideBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

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
    SetElapseSpeed(Player::GetInstance()->GetMoveVelo().Length());

    shotCoolTimer_.Update(elapseSpeed_);
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }

    //リリースでもいじりたいからifdefで囲ってない
    static float coolTimer = 60.0f;
    ImGui::Begin("SideBulletParameter");
    //1F~180Fまでの間にとどめる
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 1.0f, 180.0f);
    ImGui::End();
    shotCoolTimer_.SetMaxTimer(coolTimer);
}