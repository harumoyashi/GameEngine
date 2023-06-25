#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class Bullet
{
private:
	std::unique_ptr<NObj3d> obj;	//�e�̃I�u�W�F�N�g
	float moveAngle;				//�ړ��p�p�x
	float moveSpeed;				//�ړ��X�s�[�h

private:
	bool isActive;				//�L���t���O
	float collisionRadius;		//�R���C�_�[�̔��a
	SphereCollider collider;	//�e�̓����蔻��

	float damage;				//�^����_���[�W��

	bool isCanGenerate;			//�e�����ł���t���O

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
	inline const SphereCollider& GetBulletCollider()const { return collider; }
	//�L���t���O�擾
	inline bool GetisActive()const { return isActive; }
	//�e�����ł���t���O�擾
	inline bool GetisCanGenerate()const { return isCanGenerate; }
	//�ړ��X�s�[�h�擾
	inline float GetMoveSpeed()const { return moveSpeed; }
	//�^����_���[�W�ʎ擾
	inline float GetDamage()const { return damage; }
	//���W�擾
	inline NVector3& GetPos()const { return obj->position_; }
	//�傫���擾
	inline NVector3& GetScale()const { return obj->scale_; }
	//�ړ��p�p�x�擾
	inline float GetMoveAngle()const { return moveAngle; }

public:
	// �Z�b�^�[ //
	//�L���t���O�ݒ�
	inline void SetisActive(const bool& isActive) { this->isActive = isActive; }
	//�e�����ł���t���O�ݒ�
	inline void SetisCanGenerate(const bool& isCanGenerate) { this->isCanGenerate = isCanGenerate; }
	//�^����_���[�W�ʐݒ�
	inline void SetDamage(const float& damage) { this->damage = damage; }
	//�傫���ݒ�
	inline void SetScale(const float& scale) { obj->scale_ = scale; collisionRadius = scale; }
	//�ړ��X�s�[�h�ݒ�
	inline void SetMoveSpeed(const float& moveSpeed) { this->moveSpeed = moveSpeed; }
	//�F�ݒ�
	inline void SetColor(const NColor& color) { obj->color_ = color; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetColRadius(const float& radius) { collisionRadius = collisionRadius * radius; }
};