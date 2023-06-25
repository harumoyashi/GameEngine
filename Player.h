#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

#include "Bullet.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//�v���C���[�̓����蔻��

	bool isAlive_;		//�����Ă邩�t���O
	bool isCanMove_;	//�����邩�t���O
	bool isDraw_;		//�`�悷�邩�t���O

	float collisionRadius_;	//�R���C�_�[�̔��a

	// �_���[�W�֘A
	bool isGodmode_;		//���G��Ԃ�
	bool isFlashing_;		//�`�J�`�J�����邽�ߗp
	NTimer godmodeTimer_;	//���G����

	//�e�֘A//
	std::list<std::unique_ptr<Bullet>> bullets_;	//�e�ǂ�
	//NTimer shotTimer_;		//�e������
	NTimer shotCoolTimer_;		//�e���܂ł̎���
	bool isCanShot_;			//���Ă邩�t���O

public:
	Player();
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();

	// �Q�b�^�[ //
	//���W�擾
	inline NVector3 GetPos() { return obj_->position_; }
	//�傫���擾
	inline NVector3 GetScale() { return obj_->scale_; }
	//�R���C�_�[�̔��a�擾
	inline float GetCollisionRadius() { return collisionRadius_; }
	//�e�ǂ��擾
	inline std::list<std::unique_ptr<Bullet>>* GetBullets() { return &bullets_; }
	//���G��Ԏ擾
	inline bool GetisGodmode() { return isGodmode_; }
	//�����Ă邩�t���O�擾
	inline bool GetisAlive() { return isAlive_; }
	//�����邩�t���O�擾
	inline bool GetisCanMove() { return isCanMove_; }
	//�R���C�_�[�擾
	inline const SphereCollider& GetPlayerCollider()const { return collider_; }

	// �Z�b�^�[ //
	//���W�ݒ�
	inline void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//�傫���ݒ�
	inline void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//���G��Ԑݒ�
	inline void SetisGodmode(const bool& isGodmode) { isGodmode_ = isGodmode; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetCollisionRadius(const float& collisionRadius) { collisionRadius_ = collisionRadius; }
	//�����邩�t���O�ݒ�
	inline void SetisCanMove(const bool& isCanMove) { isCanMove_ = isCanMove; }
};