#include "Player.h"
#include "NInput.h"
#include "BulletFactory.h"

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
	obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�

	collisionRadius_ = obj_->scale_.x;

	isAlive_ = true;
	isGodmode_ = false;
	isDraw_ = true;

	moveSpeed_ = 0.05f;

	godmodeTimer_.Reset();
	godmodeTimer_.SetMaxTimer(120.0f);

	isMove_ = false;

	//�e�̃��x������
	lineLevel_ = 1;
	sideLevel_ = 1;
	wideLevel_ = 0;
	roketLevel_ = 0;
}

void Player::Update()
{
	Move();
	if (isMove_)			//���͂���Ă���
	{
		Shot();
	}

	obj_->Update();
}

void Player::Draw()
{
	if (isDraw_)
	{
		obj_->Draw();
	}
}

void Player::Move()
{
	isMove_ = false;

	//�X�e�B�b�N�ړ�
	moveVelo_ = NInput::GetStick();
	//�C�[�W���O�I�Ȋ����ɂ��đ��x�̉ϖ��m��
	moveVelo_.x *= moveVelo_.x * NInput::GetStick().x;
	moveVelo_.y *= moveVelo_.y * NInput::GetStick().y;

	if (moveVelo_.Length() > 0.0f)	//���͂���Ă���
	{
		isMove_ = true;
	}

	//�ړ��ʂ����Z
	obj_->position_.x += moveVelo_.x * moveSpeed_;
	obj_->position_.z += moveVelo_.y * moveSpeed_;

	//�ړ������ɍ��킹�ĉ�]
	if (isMove_)			//���͂���Ă���
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

void Player::Shot()
{
	BulletFactory::GetInstance()->Update();

	if (lineLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(IBullet::BulletType::LineBullet,
				obj_->position_ + NVector3(0, obj_->scale_.y, 0), lineLevel_);
	}

	if (sideLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(IBullet::BulletType::SideBullet,
				obj_->position_ + NVector3(0, obj_->scale_.y, 0), sideLevel_);
	}

	/*if (wideLevel)
	{

	}

	if (roketLevel)
	{

	}*/
}