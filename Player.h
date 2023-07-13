#pragma once
#include "NObj3d.h"
#include "NTimer.h"
#include "NCollider.h"
#include "SphereCollider.h"

class Player
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//�v���C���[�̓����蔻��

	bool isAlive_;				//�����Ă邩�t���O
	bool isDraw_;				//�`�悷�邩�t���O

	//------------------------ �ړ��֘A ------------------------//
	bool isMove_;				//�����Ă邩�t���O
	NVector2 moveVelo_;			//�ړ���
	float moveSpeed_;			//�ړ����x

	float moveAngle_;			//��]�p�x

	float elapseSpeed_;			//�o�ߎ���(�������S�Ăɉe�����y�ڂ�)

	//------------------------ �_���[�W�֘A ------------------------//
	bool isGodmode_;			//���G��Ԃ�
	bool isFlashing_;			//�`�J�`�J�����邽�ߗp
	NTimer godmodeTimer_;		//���G����

	//------------------------ �e�֘A ------------------------//
	uint32_t lineLevel_;		//�ʏ�e�̃��x��
	uint32_t sideLevel_;		//�������ɏo��e�̃��x��
	uint32_t wideLevel_;		//���E�Ɋp�x���ďo��e�̃��x��
	uint32_t roketLevel_;		//���P�b�g�e�̃��x��

public:
	Player();
	static Player* GetInstance();

	//������
	bool Init();
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�ړ�����
	void Move();
	//�ˌ�
	void Shot();

	//�����ɓ����������̏���
	void OnCollision();

	// �Q�b�^�[ //
	//���W�擾
	inline NVector3 GetPos()const { return obj_->position_; }
	//�O�����W�擾
	inline float GetFrontPosZ()const { return obj_->position_.z + obj_->scale_.z; }
	//�傫���擾
	inline NVector3 GetScale()const { return obj_->scale_; }
	//���G��Ԏ擾
	inline bool GetIsGodmode()const { return isGodmode_; }
	//�����Ă邩�t���O�擾
	inline bool GetIsAlive()const { return isAlive_; }
	//�����邩�t���O�擾
	inline bool GetIsMove()const { return isMove_; }
	//�R���C�_�[�擾
	inline const SphereCollider& GetPlayerCollider()const { return collider_; }
	//�ړ��ʎ擾
	inline NVector2 GetMoveVelo()const { return moveVelo_; }
	//�o�ߎ��Ԏ擾
	inline float GetElapseSpeed()const { return elapseSpeed_; }

	// �Z�b�^�[ //
	//���W�ݒ�
	inline void SetPos(const NVector3& pos) { obj_->position_ = pos; }
	//�傫���ݒ�
	inline void SetScale(const NVector3& scale) { obj_->scale_ = scale; }
	//���G��Ԑݒ�
	inline void SetIsGodmode(bool isGodmode) { isGodmode_ = isGodmode; }
	//�����Ă邩�t���O�ݒ�
	inline void SetIsAlive(bool isAlive) { isAlive_ = isAlive; }
	//�����邩�t���O�ݒ�
	inline void SetIsMove(bool isMove) { isMove_ = isMove; }
};