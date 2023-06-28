#include "Player.h"

Player::Player()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("cat");
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

void Player::Init()
{
	obj_->position_ = {};
	obj_->scale_ = 1.0f;

	collisionRadius_ = obj_->scale_.x;

	bullets_.clear();

	isAlive_ = true;
	isGodmode_ = false;
	isDraw_ = true;

	godmodeTimer_.Reset();
	godmodeTimer_.SetMaxTimer(120.0f);

	//shotTimer_.SetMaxTimer(10);
	shotCoolTimer_.SetMaxTimer(10);

	isCanShot_ = false;

	isCanMove_ = true;
}

void Player::Update()
{
	obj_->Update();
}

void Player::Draw()
{
	for (const auto& bullet : bullets_)
	{
		bullet->Draw();
	}

	if (isDraw_)
	{
		obj_->Draw();
	}
}
