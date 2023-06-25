#include "Player.h"

void Player::Init()
{
	obj = std::make_unique<NObj3d>();
	obj->SetModel("cat");
}

void Player::Update()
{
	obj->Update();
}

void Player::Draw()
{
	obj->Draw();
}
