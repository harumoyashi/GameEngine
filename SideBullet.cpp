#include "SideBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

SideBullet::SideBullet()
{
    shotCoolTimer_ = 60.0f;		//’eŒ‚‚Â‚Ü‚Å‚ÌŽžŠÔ
    shortShotCoolTimer_ = 5.0f; //1ƒ‹[ƒv‚Å“ñ‰ñˆÈã‘Å‚ÂŽž‚É’eŒ‚‚Â‚Ü‚Å‚ÌŽžŠÔ
    isCanShot_ = true;			//Œ‚‚Ä‚é‚©ƒtƒ‰ƒO
    level_ = 0;                 //’e‚Ì‹­‰»ƒŒƒxƒ‹
    allAtOnceNum_ = 2;          //ˆê“x‚ÉŒ‚‚Â’e‚Ì”
}

void SideBullet::SideUpdate()
{
    SetElapseSpeed(Player::GetInstance()->GetMoveVelo().Length());

    shotCoolTimer_.Update(elapseSpeed_);
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }

#ifdef _DEBUG
    ImGui::Begin("SideBullet");
    ImGui::Text("elapseSpeed:%f", elapseSpeed_);
    ImGui::Text("shotCoolTimer:%f", shotCoolTimer_.GetTimeRate());
    ImGui::End();
#endif
}