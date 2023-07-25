#include "SideBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

SideBullet::SideBullet()
{
	shotCoolTimer_ = 1.0f;					//�e���܂ł̎���
	shortShotCoolTimer_ =
		shotCoolTimer_.maxTime_ / 10.0f;	//1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
	isCanShot_ = false;						//���Ă邩�t���O
	level_ = 0;								//�e�̋������x��
	allAtOnceNum_ = 2;						//��x�Ɍ��e�̐�
}

void SideBullet::SideUpdate()
{
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	shotCoolTimer_.Update(elapseSpeed_);
	shortShotCoolTimer_.Update(elapseSpeed_);
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

	//�^�C�}�[���[�v
	if (shortShotCoolTimer_.GetStarted() == false)
	{
		shortShotCoolTimer_.Start();
	}
	if (shortShotCoolTimer_.GetEnd())
	{
		shortShotCoolTimer_.Reset();
	}

	//�����[�X�ł������肽������ifdef�ň͂��ĂȂ�
	static float coolTimer = 1.0f;
	ImGui::Begin("SideBulletParameter");
	//0�b~10�b�܂ł̊ԂɂƂǂ߂�
	ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 0.0f, 10.0f);
	ImGui::End();
	shotCoolTimer_.maxTime_ = coolTimer;
}