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

void Item::Generate(const NVec3& pos, const float moveAngle)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel("cube");
	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale();
	obj_->color_ = NColor::kYellow;

	//�R���C�_�[�ݒ�
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("item");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Item::OnCollision, this));
}

void Item::Update()
{
	//�o�ߎ��Ԃ�K�p
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

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
		obj_->Draw();
	}
}

void Item::OnCollision()
{
	//�����������肪�v���C���[���������̏���
	if (collider_.GetColInfo()->GetColID() == "player")
	{
		isAlive_ = false;
		Player::GetInstance()->LevelUp(BulletType::LineBullet);
	}
}
