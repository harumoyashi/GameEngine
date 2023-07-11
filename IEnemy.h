#pragma once
#include "NObj3d.h"
#include "NCollider.h"

class IEnemy
{
public:
	//�����̃^�C�v�ɉ����ăt�@�N�g���[�Ŕz�u���Đ�������
	enum class EnemyType
	{
		WolfSide,	//�����炭��T����
	};

protected:
	std::unique_ptr<NObj3d> obj_;	//�I�u�W�F�N�g
	NVector2 moveVelo_;				//�ړ���
	float moveAngle_;				//�ړ��p�p�x
	float moveSpeed_;				//�ړ��X�s�[�h

	bool isAlive_;					//�����t���O
	uint32_t maxHP_;				//�ő�̗�
	uint32_t hp_;					//���݂̗̑�
	float collisionRadius_;			//�R���C�_�[�̔��a
	Sphere collider_;		//�����蔻��

	float elapseSpeed_;				//�o�ߎ��Ԃ̃X�s�[�h(�X���[���[�V�����p)

	//�A�C�e����������

	//�R�C��(�Q�[�W)��������


public:
	IEnemy();
	virtual ~IEnemy() = default;

	//������
	//pos:�z�u������W
	//moveAngle:�ړ�����p�x
	//modelname:���f���̖��O(ID)
	void Generate(const NVector3& pos, const float moveAngle, const std::string& modelname);
	//�X�V
	void Update();
	//�`��
	void Draw();

	//---------------------------- �p�������� ----------------------------//
	//�ړ�
	virtual void Move();

	// �Q�b�^�[ //
	//�R���C�_�[�擾
	inline const Sphere& GetCollider()const { return collider_; }
	//�����t���O�擾
	inline bool GetisAlive()const { return isAlive_; }
	//�ړ��X�s�[�h�擾
	inline float GetMoveSpeed()const { return moveSpeed_; }
	//���W�擾
	inline NVector3& GetPos()const { return obj_->position_; }
	//�傫���擾
	inline NVector3& GetScale()const { return obj_->scale_; }
	//�ړ��p�p�x�擾
	inline float GetMoveAngle()const { return moveAngle_; }

	// �Z�b�^�[ //
	//�����t���O�ݒ�
	inline void SetisAlive(const bool isAlive) { isAlive_ = isAlive; }
	//���W�ݒ�
	inline void SetPos(const NVector3 pos) { obj_->position_ = pos; collider_.centerPos = pos; }
	//�傫���ݒ�
	inline void SetScale(const float scale) {
		obj_->scale_ = scale; collisionRadius_ = scale; collider_.radius = scale; }
	//�ړ��p�x�ݒ�
	inline void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//�ړ��X�s�[�h�ݒ�
	inline void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//�R���C�_�[�̔��a�ݒ�
	inline void SetColRadius(const float radius) { collisionRadius_ = collisionRadius_ * radius; }
	//�o�ߎ��ԃX�s�[�h�ݒ�
	inline void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};