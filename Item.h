#pragma once
#include "NObj3d.h"
#include "SphereCollider.h"
#include "NEasing.h"

class Item
{
private:
	std::unique_ptr<NObj3d> obj_;	//�I�u�W�F�N�g
	NVec2 moveVelo_;				//�ړ���
	float moveSpeed_;				//�ړ��X�s�[�h

	bool isAlive_;					//�����t���O
	SphereCollider collider_;		//�����蔻��

	float elapseSpeed_;				//�o�ߎ��Ԃ̃X�s�[�h(�X���[���[�V�����p)

public:
	Item();
	~Item();

	//����
	void Generate(const NVec3& pos, const float moveAngle = 0.0f);
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

	// �Z�b�^�[ //
	//�����t���O�ݒ�
	void SetisAlive(bool isAlive) { isAlive_ = isAlive; }
	//�o�ߎ��ԃX�s�[�h�ݒ�
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};