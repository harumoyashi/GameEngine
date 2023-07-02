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

#ifdef _DEBUG
    ImGui::Begin("SideBullet");
    ImGui::Text("elapseSpeed:%f", elapseSpeed_);
    ImGui::Text("shotCoolTimer:%f", shotCoolTimer_.GetTimeRate());
    ImGui::End();
#endif
}