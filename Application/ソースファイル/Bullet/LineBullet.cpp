#include "LineBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

LineBullet::LineBullet()
{
    shotCoolTimer_ = 0.1f;      //�e���܂ł̎���
    isCanShot_ = false;			//���Ă邩�t���O
    level_ = 1;                 //�e�̋������x��
    
    //obj_->color_ = NColor::kRed;  //�Ȃ����G���[�o��
    SetDamage(1.0f);
}

void LineBullet::LineUpdate()
{
    SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

    shotCoolTimer_.Update(elapseSpeed_);
    //�^�C�}�[���[�v
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
    //0�b~10�b�܂ł̊ԂɂƂǂ߂�
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 0.0f, 10.0f);
    ImGui::End();
    shotCoolTimer_.maxTime_ = coolTimer;
#endif // DEBUG
}
