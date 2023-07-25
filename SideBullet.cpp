#include "SideBullet.h"
#include "Player.h"

#include "NImGuiManager.h"
#include "imgui.h"

SideBullet::SideBullet()
{
	shotCoolTimer_ = 1.0f;					//弾撃つまでの時間
	shortShotCoolTimer_ =
		shotCoolTimer_.maxTime_ / 10.0f;	//1ループで二回以上打つ時に弾撃つまでの時間
	isCanShot_ = false;						//撃てるかフラグ
	level_ = 0;								//弾の強化レベル
	allAtOnceNum_ = 2;						//一度に撃つ弾の数
}

void SideBullet::SideUpdate()
{
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	shotCoolTimer_.Update(elapseSpeed_);
	shortShotCoolTimer_.Update(elapseSpeed_);
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

	//タイマーループ
	if (shortShotCoolTimer_.GetStarted() == false)
	{
		shortShotCoolTimer_.Start();
	}
	if (shortShotCoolTimer_.GetEnd())
	{
		shortShotCoolTimer_.Reset();
	}

	//リリースでもいじりたいからifdefで囲ってない
	static float coolTimer = 1.0f;
	ImGui::Begin("SideBulletParameter");
	//0秒~10秒までの間にとどめる
	ImGui::SliderFloat("ShotCoolTimer", &coolTimer, 0.0f, 10.0f);
	ImGui::End();
	shotCoolTimer_.maxTime_ = coolTimer;
}