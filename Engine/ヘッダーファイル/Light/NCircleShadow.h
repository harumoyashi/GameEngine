#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NCircleShadow final
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	std::unique_ptr<NConstBuff<ConstBuffDataCircleShadow>> cbCircleShadow_;

private://�ÓI�����o�ϐ�
	NVector3 dir_ = { 1,0,0 };			// ����
	float distanceCasterLight_ = 100.0f;	// �L���X�^�[�ƃ��C�g�̋���
	NVector3 casterPos_ = { 0,0,0 };		// �L���X�^�[���W(���[���h)
	NVector3 atten_ = { 0.5f,0.6f,0 };			// ���������W��
	NVector2 factorAngleCos_ = { 0.2f,0.5f };	// �����p�x

	//�_�[�e�B�t���O
	bool isDirty_ = false;
	//�L���t���O
	bool isActive_ = false;

public://�����o�֐�
	NCircleShadow();
	~NCircleShadow();

	//������
	void Init();

	//�e�̕������Z�b�g
	void SetDir(const NVector3& dir);
	const NVector3& GetDir()const { return dir_; }
	//���W���Z�b�g
	void SetCasterPos(const NVector3& casterPos);
	const NVector3& GetCasterPos()const { return casterPos_; }
	//�L���X�^�[�ƃ��C�g�̋������Z�b�g
	void SetDistanceCasterLight(const float distanceCasterLight);
	const float GetDistanceCasterLight()const { return distanceCasterLight_; }
	//�����W�����Z�b�g
	void SetAtten(const NVector3& atten);
	const NVector3& GetAtten()const { return atten_; }
	//�����p�x���Z�b�g
	void SetFactorAngle(const NVector2& factorAngle);
	const NVector2& GetFactorAngle()const { return factorAngleCos_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};