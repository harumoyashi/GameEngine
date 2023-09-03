#pragma once
#include "NObj3d.h"
#include "SphereCollider.h"
#include "NEasing.h"

enum class BulletType
{
	LineBullet,
	SideBullet,
	WideBullet,
	//Roket,

	MaxType
};

class IBullet
{
protected:
	BulletType bulletType;			//�e�̎��

	std::unique_ptr<NObj3d> obj_;	//�e�̃I�u�W�F�N�g
	NVec2 moveVelo_;				//�ړ���
	float moveAngle_;				//�ړ��p�p�x
	float moveSpeed_;				//�ړ��X�s�[�h

	bool isAlive_;					//�����t���O
	SphereCollider collider_;		//�e�̓����蔻��

	NEasing::EaseTimer lifeTimer_;	//��������

	float damage_;					//�^����_���[�W��

	float elapseSpeed_;				//�o�ߎ��Ԃ̃X�s�[�h(�X���[���[�V�����p)

public:
	IBullet();
	virtual ~IBullet();
	//����
	void Generate(const NVec3& pos,const float moveAngle = 0.0f);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�����ɓ����������̏���
	void OnCollision();

public:
	// �Q�b�^�[ //
	//�R���C�_�[�擾
	const SphereCollider& GetIBulletCollider()const { return collider_; }
	//�����t���O�擾
	bool GetisAlive()const { return isAlive_; }
	//�ړ��X�s�[�h�擾
	float GetMoveSpeed()const { return moveSpeed_; }
	//�^����_���[�W�ʎ擾
	float GetDamage()const { return damage_; }
	//���W�擾
	NVec3& GetPos()const { return obj_->position_; }
	//�傫���擾
	NVec3& GetScale()const { return obj_->scale_; }
	//�ړ��p�p�x�擾
	float GetMoveAngle()const { return moveAngle_; }

	// �Z�b�^�[ //
	//�����t���O�ݒ�
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//�^����_���[�W�ʐݒ�
	void SetDamage(const float damage) { damage_ = damage; }
	//�傫���ݒ�
	void SetScale(const float scale) { obj_->scale_ = scale; collider_.SetRadius(scale); }
	//�ړ��p�x�ݒ�
	void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//�ړ��X�s�[�h�ݒ�
	void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//�������Ԑݒ�
	void SetLifeTimer(const float lifeTimer) { lifeTimer_ = lifeTimer; }
	//�o�ߎ��ԃX�s�[�h�ݒ�
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};