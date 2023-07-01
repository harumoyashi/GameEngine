#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//�v���C���[�̓����蔻��

	bool isAlive_;		//�����Ă邩�t���O
	bool isDraw_;		//�`�悷�邩�t���O

	float collisionRadius_;	//�R���C�_�[�̔��a

	//------------------------ �ړ��֘A ------------------------//
	bool isMove_;				//�����Ă邩�t���O
	NVector2 moveVelo_;			//�ړ���
	float moveSpeed_ = 0.5f;	//�ړ����x

	float angle_;				//��]�p�x

	//------------------------ �_���[�W�֘A ------------------------//
	bool isGodmode_;		//���G��Ԃ�
	bool isFlashing_;		//�`�J�`�J�����邽�ߗp
	NTimer godmodeTimer_;	//���G����

	//------------------------ �e�֘A ------------------------//
	uint32_t lineLevel_;		//�ʏ�e�̃��x��
	uint32_t sideLevel_;		//�������ɏo��e�̃��x��
	uint32_t wideLevel_;		//���E�Ɋp�x���ďo��e�̃��x��
	uint32_t roketLevel_;		//���P�b�g�e�̃��x��

public:
	Player();
	static Player* GetInstance();

	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�ړ�����
	void Move();
	//�ˌ�
	void Shot();

	// �Q�b�^�[ //
	//���W�擾
	inline NVector3 GetPos() { return obj_->position_; }
	//�傫���擾
	inline NVector3 GetScale() { return obj_->scale_; }
	//�R���C�_�[�̔��a�擾
	inline float GetCollisionRadius() { return collisionRadius_; }
	//���G��Ԏ擾
	inline bool GetIsGodmode() { return isGodmode_; }
	//�����Ă邩�t���O�擾
	inline bool GetIsAlive() { return isAlive_; }
	//�����邩�t���O�擾
	inline bool GetIsMove() { return isMove_; }
	//�R���C�_�[�擾
	inline const SphereCollider& GetPlayerCollider()const { return collider_; }
	//�ړ��ʎ擾
	inline NVector2 GetMoveVelo() { return moveVelo_; }

	// �Z�b�^�[ //
	//���W�ݒ�
	inline void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//�傫���ݒ�
	inline void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//���G��Ԑݒ�
	inline void SetIsGodmode(const bool& isGodmode) { isGodmode_ = isGodmode; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetCollisionRadius(const float& collisionRadius) { collisionRadius_ = collisionRadius; }
	//�����邩�t���O�ݒ�
	inline void SetIsMove(const bool& isMove) { isMove_ = isMove; }
};