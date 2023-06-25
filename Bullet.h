#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class Bullet
{
private:
	std::unique_ptr<NObj3d> obj_;	//�e�̃I�u�W�F�N�g
	float moveAngle_;				//�ړ��p�p�x
	float moveSpeed_;				//�ړ��X�s�[�h

private:
	bool isActive_;				//�L���t���O
	float collisionRadius_;		//�R���C�_�[�̔��a
	SphereCollider collider_;	//�e�̓����蔻��

	float damage_;				//�^����_���[�W��

	bool isCanGenerate_;		//�e�����ł���t���O

public:
	Bullet();
	//����
	void Generate(const NVector3& pos);
	//�X�V
	void Update();
	//�`��
	void Draw();
public:
	// �Q�b�^�[ //
	//�R���C�_�[�擾
	inline const SphereCollider& GetBulletCollider()const { return collider_; }
	//�L���t���O�擾
	inline bool GetisActive()const { return isActive_; }
	//�e�����ł���t���O�擾
	inline bool GetisCanGenerate_()const { return isCanGenerate_; }
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
	//�L���t���O�ݒ�
	inline void SetisActive(const bool isActive) { isActive_ = isActive; }
	//�e�����ł���t���O�ݒ�
	inline void SetisCanGenerate_(const bool isCanGenerate) { isCanGenerate_ = isCanGenerate; }
	//�^����_���[�W�ʐݒ�
	inline void SetDamage(const float damage) { damage_ = damage; }
	//�傫���ݒ�
	inline void SetScale(const float scale) { obj_->scale_ = scale; collisionRadius_ = scale; }
	//�ړ��X�s�[�h�ݒ�
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//�F�ݒ�
	inline void SetColor(const NColor& color) { obj_->color_ = color; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
};