#include "SideBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

SideBullet::SideBullet()
{
    shotCoolTimer_ = 60.0f;		//�e���܂ł̎���
    shortShotCoolTimer_ = 5.0f; //1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    isCanShot_ = true;			//���Ă邩�t���O
    level_ = 0;                 //�e�̋������x��
    allAtOnceNum_ = 2;          //��x�Ɍ��e�̐�
}

void SideBullet::SideUpdate()
{
    SetElapseSpeed(Player::GetInstance()->GetMoveVelo().Length());

    shotCoolTimer_.Update(elapseSpeed_);
    if (shotCoolTimer_.GetisTimeOut())
    {
        isCanShot_ = true;
    }

    //�����[�X�ł������肽������ifdef�ň͂��ĂȂ�
    static float coolTimer = 60.0f;
    ImGui::Begin("SideBulletParameter");
    //1F~180F�܂ł̊ԂɂƂǂ߂�
    ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 1.0f, 180.0f);
    ImGui::End();
    shotCoolTimer_.SetMaxTimer(coolTimer);
}