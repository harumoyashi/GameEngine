#include "LineBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

LineBullet::LineBullet()
{
    bulletType = BulletType::LineBullet;
    shotCoolTimer_ = 0.1f;      //弾撃つまでの時間
    isCanShot_ = false;			//撃てるかフラグ
    level_ = 1;                 //弾の強化レベル

    SetDamage(1.0f);
}

void LineBullet::LineUpdate()
{
    SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

    shotCoolTimer_.Update(elapseSpeed_);
    //タイマーループ
    if (shotCoolTimer_.GetStarted() == false)
    {
        shotCoolTimer_.Start();
    }
    if (shotCoolTimer_.GetEnd())
    {
        isCanShot_ = true;
        shotCoolTimer_.Reset();
    }

#ifdef _DEBUG
    static float coolTimer = 0.1f;
    ImGui::Begin("LineBulletParameter");
    //0秒~10秒までの間にとどめる
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 0.0f, 10.0f);
    ImGui::End();
    shotCoolTimer_.maxTime_ = coolTimer;
#endif // DEBUG
}
