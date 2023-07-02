#include "LineBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 5.0f;      //�e���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 1;                 //�e�̋������x��
    
    //obj_->color_ = NColor::kRed;  //�Ȃ����G���[�o��
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

    //�����[�X�ł������肽������ifdef�ň͂��ĂȂ�
    static float coolTimer = 5.0f;
    ImGui::Begin("LineBulletParameter");
    //1F~60F�܂ł̊ԂɂƂǂ߂�
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 1.0f, 60.0f);
    ImGui::End();
    shotCoolTimer_.SetMaxTimer(coolTimer);
}
