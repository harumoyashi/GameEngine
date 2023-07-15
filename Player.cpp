#include "Player.h"
#include "NInput.h"
#include "BulletFactory.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Player::Player()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("cat");

	//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɓo�^
	NParticleManager::GetInstance()->AddEmitter(&deadParticle_,"playerDead");
}

Player::~Player()
{
	//�R���C�_�[�}�l�[�W���[����폜
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	//�p�[�e�B�N���}�l�[�W���[����폜
	NParticleManager::GetInstance()->EraseEmitter("playerDead");
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

bool Player::Init()
{
	obj_->position_ = {};
	obj_->scale_ = 0.1f;
	obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�
	obj_->Update();

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

	//�R���C�_�[�ݒ�
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("player");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Player::OnCollision, this));

	return true;
}

void Player::Update()
{
	if (isAlive_)
	{
		Move();
		Shot();
	}
	else
	{
		elapseSpeed_ = 0.01f;	//���񂾂�X���[���[�V������
	}

	obj_->Update();
	collider_.Update(obj_.get());
}

void Player::Draw()
{
	if (isAlive_)	//�ق�Ƃ͈Ⴄ���ǈ�U�킩��₷���̂�
	{
		if (isDraw_)
		{
			obj_->Draw();
		}
	}
}

void Player::Move()
{
	isMove_ = false;

	//�p�b�h�ڑ�����Ă�Ȃ�
	if (NInput::GetIsConnect())
	{
		//�X�e�B�b�N�ړ�
		moveVelo_ = NInput::GetStick();
		//�C�[�W���O�I�Ȋ����ɂ��đ��x�̉ϖ��m��
		moveVelo_.x *= moveVelo_.x * NInput::GetStick().x;
		moveVelo_.y *= moveVelo_.y * NInput::GetStick().y;
	}
	else
	{
		moveVelo_ = { 0,0 };
		//�����ꂩ�̃L�[���������Ƃ�
		if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
		{
			if (NInput::IsKey(DIK_W)) { moveVelo_.y = +1.0f; }
			else if (NInput::IsKey(DIK_S)) { moveVelo_.y = -1.0f; }
			if (NInput::IsKey(DIK_D)) { moveVelo_.x = +1.0f; }
			else if (NInput::IsKey(DIK_A)) { moveVelo_.x = -1.0f; }
		}
	}

	if (moveVelo_.Length() > 0.0f)	//���͂���Ă���
	{
		isMove_ = true;
	}

	elapseSpeed_ = moveVelo_.Length();	//�ړ��ʂɂ���Čo�ߎ��ԕω�

	//�ړ��ʂ����Z
	obj_->position_.x += moveVelo_.x * moveSpeed_;
	obj_->position_.z += moveVelo_.y * moveSpeed_;

	//�ړ������ɍ��킹�ĉ�]
	if (isMove_)			//���͂���Ă���
	{
		NVector2 velo = moveVelo_;	//moveVelo_�̒l���ς��Ȃ��悤�Ɋi�[
		velo.Normalize();
		moveAngle_ = MathUtil::Radian2Degree(acosf(velo.Dot({ 0,1 })));
		if (velo.x < 0)
		{
			moveAngle_ = -moveAngle_;
		}

		obj_->rotation_.y = moveAngle_;
	}

	//�����[�X�ł������肽������ifdef�ň͂��ĂȂ�
	static int lineLv = 1, sideLv = 1;
	ImGui::Begin("PlayerParameter");
	ImGui::SliderFloat("MoveSpeed", &moveSpeed_, 0.01f, 1.0f);
	ImGui::SliderInt("LineLevel", &lineLv, 0, 5);
	ImGui::SliderInt("SideLevel", &sideLv, 0, 5);
	ImGui::End();
	lineLevel_ = lineLv;
	sideLevel_ = sideLv;
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

void Player::OnCollision()
{
	//�����������肪�G���������̏���
	if (collider_.GetColInfo()->GetColID() == "enemy")
	{
		DeadParticle();
		isAlive_ = false;
	}
}

void Player::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->emitters_["playerDead"]->SetIsRotation(true);
		NParticleManager::GetInstance()->emitters_["playerDead"]->SetPos(GetPos());
		NParticleManager::GetInstance()->emitters_["playerDead"]->Add(
			150, 100, obj_->color_, 0.1f, 1.0f, { -1,-1,-1 }, { 1,1,1 }, { 0,0,0 }, { -1,-1,-1 }, { 1,1,1 });
	}
}
