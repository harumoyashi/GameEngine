#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"

class IEnemy
{
public:
	//�����̃^�C�v�ɉ����ăt�@�N�g���[�Ŕz�u���Đ�������
	enum class EnemyType
	{
		MouseSide,	//�����炭��T����
	};

protected:
	std::unique_ptr<NObj3d> obj_;	//�I�u�W�F�N�g
	SphereCollider collider_;		//�R���C�_�[
	NVec3 oriScale_;				//���̃X�P�[���ۑ��p(���Y����炷�Ƃ����炷����)
	NVec3 addScale_;				//�X�P�[��������p(���Y����炷�Ƃ����炷����)
	NVec2 moveVelo_;				//�ړ���
	float moveAngle_;				//�ړ��p�p�x
	float moveSpeed_;				//�ړ��X�s�[�h

	bool isAlive_;					//�����t���O
	uint32_t maxHP_;				//�ő�̗�
	uint32_t hp_;					//���݂̗̑�
	uint32_t score_;				//�|�����ƂŊl���ł��链�_(�{�Ƃ�10�{�ɂ���)

	float elapseSpeed_;				//�o�ߎ��Ԃ̃X�s�[�h(�X���[���[�V�����p)

	std::string enemyTypeName_;		//�G�̎푰��
	uint32_t enemyNum_;				//�G�̎��ʔԍ�
	SimpleParticle deadParticle_;	//���񂾂Ƃ��ɏo���p�[�e�B�N��

	bool isItem_;					//�A�C�e�������Ă邩�t���O

public:
	IEnemy();
	virtual ~IEnemy();

	//����
	//pos:�z�u������W
	//moveAngle:�ړ�����p�x
	//modelname:���f���̖��O(ID)
	void Generate(const NVec3& pos, const float moveAngle, const std::string& modelname);

	//������
	virtual void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();

	//�����ɓ����������̏���
	void OnCollision();

	//�G�~�b�^�[�̓o�^
	//enemyNum:�G�̎��ʔԍ�
	void AddEmitter(uint32_t eneNum);
	//���񂾂Ƃ��̃p�[�e�B�N�����o��
	void DeadParticle();

	//---------------------------- �p�������� ----------------------------//
	//�ړ�
	virtual void Move();

	// �Q�b�^�[ //
	//�R���C�_�[�擾
	const SphereCollider GetCollider()const { return collider_; }
	//�����t���O�擾
	bool GetisAlive()const { return isAlive_; }
	//�ړ��X�s�[�h�擾
	float GetMoveSpeed()const { return moveSpeed_; }
	//���W�擾
	NVec3& GetPos()const { return obj_->position_; }
	//�傫���擾
	NVec3& GetScale()const { return obj_->scale_; }
	//�ړ��p�p�x�擾
	float GetMoveAngle()const { return moveAngle_; }
	//�p�[�e�B�N����1���o�ĂȂ����t���O�擾
	bool GetParticlesDead()const { return deadParticle_.GetParticlesDead(); }

	// �Z�b�^�[ //
	//�����t���O�ݒ�
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//���W�ݒ�
	void SetPos(const NVec3 pos) { obj_->position_ = pos; collider_.SetCenterPos(pos); }
	//�傫���ݒ�
	void SetScale(const float scale) {
		obj_->scale_ = scale;  collider_.SetRadius(scale); }
	//�X�P�[��������p�ϐ��ݒ�
	void SetAddScale(const float scale) {addScale_ = scale;}
	//�ړ��p�x�ݒ�
	void SetMoveAngle(const float moveAngle) { moveAngle_ = moveAngle; }
	//�ړ��X�s�[�h�ݒ�
	void SetMoveSpeed(const float moveSpeed) { moveSpeed_ = moveSpeed; }
	//�o�ߎ��ԃX�s�[�h�ݒ�
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
	//�G�̎��ʔԍ��ݒ�
	void SetEnemyNum(const uint32_t eneNum) { enemyNum_ = eneNum; }
	//�A�C�e�������Ă邩�t���O�ݒ�
	void SetisItem(bool isItem);
};