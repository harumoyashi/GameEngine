#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "NTimer.h"

class IBullet
{
public:
	enum class BulletType
	{
		LineBullet,
		SideBullet,
		WideBullet,
		Roket,
	};

protected:
	std::unique_ptr<NObj3d> obj_;	//�e�̃I�u�W�F�N�g
	NVector2 moveVelo_;				//�ړ���
	float moveAngle_;				//�ړ��p�p�x
	float moveSpeed_;				//�ړ��X�s�[�h

	bool isAlive_;				//�����t���O
	float collisionRadius_;		//�R���C�_�[�̔��a
	SphereCollider collider_;	//�e�̓����蔻��

	NTimer lifeTimer_;			//��������

	float damage_;				//�^����_���[�W��

public:
	IBullet();
	//����
	void Generate(const NVector3& pos);
	//�X�V
	void Update();
	//�`��
	void Draw();
public:
	// �Q�b�^�[ //
	//�R���C�_�[�擾
	inline const SphereCollider& GetIBulletCollider()const { return collider_; }
	//�����t���O�擾
	inline bool GetisAlive()const { return isAlive_; }
	//�ړ��X�s�[�h�擾
	inline float GetMoveSpeed()const { return moveSpeed_; }
	//�^����_���[�W�ʎ擾
	inline float GetDamage()const { return damage_; }
	//���W�擾
	inline NVector3& GetPos()const { return obj_->position_; }
	//�傫���擾
	inline NVector3& GetScale()const { return obj_->scale_; }
	//�ړ��p�p�x�擾
	inline float GetMoveAngle()const { return moveAngle_; }

public:
	// �Z�b�^�[ //
	//�����t���O�ݒ�
	inline void SetisAlive(const bool isAlive) { isAlive_ = isAlive; }
	//�^����_���[�W�ʐݒ�
	inline void SetDamage(const float damage) { damage_ = damage; }
	//�傫���ݒ�
	inline void SetScale(const float scale) { obj_->scale_ = scale; collisionRadius_ = scale; }
	//�ړ��p�x�ݒ�
	inline void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//�ړ��X�s�[�h�ݒ�
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//�������Ԑݒ�
	inline void SetLifeTimer(const float lifeTimer) { lifeTimer_ = lifeTimer; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
};