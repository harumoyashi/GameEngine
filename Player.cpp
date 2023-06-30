#include "Player.h"
#include "NInput.h"

#include "NImGuiManager.h"
#include "imgui.h"

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
	Move();

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

void Player::Move()
{
	//スティック移動
	moveVelo_ = NInput::GetStick();

	//移動量を加算
	obj_->position_.x += moveVelo_.x * moveSpeed_;
	obj_->position_.z += moveVelo_.y * moveSpeed_;

	//移動方向に合わせて回転
	if (moveVelo_.Length() > 0.0f)	//入力されてたら
	{
		moveVelo_.Normalize();
		angle_ = MathUtil::Radian2Degree(acosf(moveVelo_.Dot({ 0,1 })));
		if (moveVelo_.x < 0)
		{
			angle_ = -angle_;
		}

		obj_->rotation_.y = angle_;
	}

#ifdef _DEBUG
	ImGui::Begin("Rot");
	ImGui::Text("rot:%f", obj_->rotation_.y);
	ImGui::End();
#endif
}
