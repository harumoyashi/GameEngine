#include "IBullet.h"
#include "Player.h"
#include "NCollisionManager.h"

IBullet::IBullet() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.2f), isAlive_(true),
	lifeTimer_(120.0f), damage_(1), elapseSpeed_(0.0f)
{
}

IBullet::~IBullet()
{
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

void IBullet::Generate(const NVector3& pos, const float moveAngle)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("Sphere");
	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 0.5f;
	obj_->color_ = NColor::kWhite;
	
	//�R���C�_�[�ݒ�
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("bullet");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IBullet::OnCollision, this));

	moveAngle_ = moveAngle;
}

void IBullet::Update()
{
	//�o�ߎ��Ԃ�K�p
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//�������ԉ߂����玀�񂾔����
	lifeTimer_.Update(elapseSpeed_);
	if (lifeTimer_.GetisTimeOut())
	{
		isAlive_ = false;
	}

	//�e�̈ړ�
	//�ړ��� = �i�ޕ��� * �X�s�[�h * �o�ߎ���
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//�ړ�
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()�ŌĂԂƁA���̃t���[���ł̑�������ɉe�����o�邩�炱���ŏ����Ă�
	if (isAlive_ == false)
	{
		//�R���C�_�[�}�l�[�W���[����폜
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void IBullet::Draw()
{
	obj_->Draw();
}

void IBullet::OnCollision()
{
	//�����������肪�e���������̏���
	if (collider_.GetColInfo()->GetColID() == "enemy")
	{
		isAlive_ = false;
	}
}