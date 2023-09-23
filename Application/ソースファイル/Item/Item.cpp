#include "Item.h"
#include "Player.h"
#include "NCollisionManager.h"

Item::Item() :
	moveVelo_({ 0,0 }), moveSpeed_(0.2f), isAlive_(true), elapseSpeed_(0.0f)
{
}

Item::~Item()
{
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

void Item::Generate(const NVec3& pos)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("cube");
	obj_->SetTexture("item");
	obj_->position_ = pos;
	obj_->rotation_.x = 45.f;
	obj_->rotation_.z = 45.f;
	obj_->scale_ = Player::GetInstance()->GetScale() * 2.0f;

	//�R���C�_�[�ݒ�
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x * 1.5f);	//���傢�傫�߂ɔ�����
	collider_.SetColID("item");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Item::OnCollision, this));

	moveVelo_ = { 0.f,0.8f };
}

void Item::Update()
{
	//�o�ߎ��Ԃ�K�p
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//���邭��܂킷
	obj_->rotation_.y += 5.f * elapseSpeed_;

	//������
	obj_->position_.y += moveVelo_.y * elapseSpeed_;
	//�d�͉��Z
	moveVelo_.y -= 0.1f * elapseSpeed_;
	//�s�������Ȃ��悤��
	obj_->position_.y = MathUtil::Clamp(obj_->position_.y, collider_.GetRadius(), 100.f);

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()�ŌĂԂƁA���̃t���[���ł̑�������ɉe�����o�邩�炱���ŏ����Ă�
	if (isAlive_ == false)
	{
		//�R���C�_�[�}�l�[�W���[����폜
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void Item::Draw()
{
	if (isAlive_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void Item::OnCollision()
{
	//�����������肪�v���C���[���������̏���
	if (collider_.GetColInfo()->GetColID() == "player")
	{
		isAlive_ = false;
		Player::GetInstance()->LevelUp(bulletType_);
	}
}
