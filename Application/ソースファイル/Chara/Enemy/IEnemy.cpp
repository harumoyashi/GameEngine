#include "IEnemy.h"
#include "Player.h"
#include "Field.h"
#include "NCamera.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "ItemManager.h"
#include "Score.h"

//�X�s�[�h�͊�{�v���C���[��肿�傢�x��
IEnemy::IEnemy() :
	moveVelo_({ 0,0 }), moveAngle_(0.0f), moveSpeed_(0.04f), isAlive_(true),
	elapseSpeed_(0.0f), maxHP_(1), hp_(maxHP_),score_(10), isItem_(false)
{
}

IEnemy::~IEnemy()
{
}

void IEnemy::Generate(const NVec3& pos, const float moveAngle, const std::string& modelname)
{
	obj_ = move(std::make_unique<NObj3d>());
	obj_->SetModel(modelname);

	obj_->position_ = pos;
	obj_->scale_ = Player::GetInstance()->GetScale() * 1.5f;
	obj_->color_ = NColor::kLightblue;
	obj_->Update();

	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("enemy");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&IEnemy::OnCollision, this));

	moveAngle_ = moveAngle;

	NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetIsRotation(true);
}

void IEnemy::Init()
{
}

void IEnemy::Update()
{
	//�o�ߎ��Ԃ�K�p
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//�ړ�
	Move();

	// �u2D�ɒ������s���͈�+��ʒ[���W�v���u2D�ɒ������G�̍��W+�G�̔��a�v���������ꍇ�E�� //
	float borderLineRight, borderLineLeft;	//�������玀�ʂƂ�

	//�u2D�ɒ������s���͈�+��ʒ[���W�v
	//�s���͈͂��X�N���[�����W�ɕϊ����ăE�B���h�E���W�Ƒ���
	NMatrix4 matWorldRight, matWorldLeft;

	matWorldRight = matWorldRight.Translation(NVec3(Field::GetInstance()->GetActivityAreaX(), 0, 0));
	borderLineRight = MathUtil::WorldToScreen(NVec3(), matWorldRight).x + NWindows::kWin_width * 0.5f;

	matWorldLeft = matWorldLeft.Translation(NVec3(-Field::GetInstance()->GetActivityAreaX(), 0, 0));
	borderLineLeft = MathUtil::WorldToScreen(NVec3(), matWorldLeft).x - NWindows::kWin_width * 0.5f;

	float objRight, objLeft;	//�I�u�W�F�̉E�[���[
	objRight = MathUtil::WorldToScreen(obj_->position_ + obj_->scale_, obj_->GetMatWorld()).x;
	objLeft = MathUtil::WorldToScreen(obj_->position_ - obj_->scale_, obj_->GetMatWorld()).x;
	//�{�[�_�[���C����������E��
	if (borderLineRight < objLeft || borderLineLeft > objRight)
	{
		isAlive_ = false;
	}

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()�ŌĂԂƁA���̃t���[���ł̑�������ɉe�����o�邩�炱���ŏ����Ă�
	if (isAlive_ == false)
	{
		//�R���C�_�[�}�l�[�W���[����폜
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);
	}
}

void IEnemy::Draw()
{
	if (isAlive_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void IEnemy::OnCollision()
{
	//�����������肪�e���������̏���
	if (collider_.GetColInfo()->GetColID() == "bullet")
	{
		DeadParticle();
		isAlive_ = false;
		Score::AddScore(score_);
		//�A�C�e�������Ă�t���O�����Ă���A�C�e�����Ƃ�
		if (isItem_)
		{
			uint32_t bulType = MathUtil::Random(0,(uint32_t)BulletType::MaxType);
			ItemManager::GetInstance()->Generate(obj_->position_, (BulletType)bulType);
		}
		NAudioManager::Play("vanishSE");
	}
}

void IEnemy::AddEmitter(uint32_t eneNum)
{
	//���ʔԍ�������
	enemyNum_ = eneNum;
	//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɓo�^
	NParticleManager::GetInstance()->enemyEmitters_.emplace_back();
	NParticleManager::GetInstance()->enemyEmitters_.back() = &deadParticle_;

}

void IEnemy::DeadParticle()
{
	if (isAlive_)
	{
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->SetPos(GetPos());
		NParticleManager::GetInstance()->enemyEmitters_[enemyNum_]->Add(
			15, 0.25f, obj_->color_, 0.1f, 0.4f, { -0.5f,-0.1f,-0.5f }, { 0.5f,0.1f,0.5f }, NVec3::zero, -NVec3::one, NVec3::one);
	}
}

void IEnemy::Move()
{
	moveVelo_.x = sinf(moveAngle_) * moveSpeed_ * elapseSpeed_;
	moveVelo_.y = cosf(moveAngle_) * moveSpeed_ * elapseSpeed_;

	//�ړ�
	obj_->position_.x += moveVelo_.x;
	obj_->position_.z += moveVelo_.y;

	//�������Ă���Ƃ͋t���ۂ�
	obj_->rotation_.y = -MathUtil::Radian2Degree(moveAngle_);
}

void IEnemy::SetisItem(bool isItem)
{
	isItem_ = isItem;
	obj_->color_ = NColor::kYellow;	//�A�C�e�������Ă�G�͉��F�ɂ���
}
