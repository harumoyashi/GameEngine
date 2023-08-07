#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NVec2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NCircleShadow final
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	std::unique_ptr<NConstBuff<ConstBuffDataCircleShadow>> cbCircleShadow_;

private://�ÓI�����o�ϐ�
	NVec3 dir_ = { 1,0,0 };			// ����
	float distanceCasterLight_ = 100.0f;	// �L���X�^�[�ƃ��C�g�̋���
	NVec3 casterPos_ = NVec3::zero;		// �L���X�^�[���W(���[���h)
	NVec3 atten_ = { 0.5f,0.6f,0 };			// ���������W��
	NVec2 factorAngleCos_ = { 0.2f,0.5f };	// �����p�x

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
	void SetDir(const NVec3& dir);
	const NVec3& GetDir()const { return dir_; }
	//���W���Z�b�g
	void SetCasterPos(const NVec3& casterPos);
	const NVec3& GetCasterPos()const { return casterPos_; }
	//�L���X�^�[�ƃ��C�g�̋������Z�b�g
	void SetDistanceCasterLight(const float distanceCasterLight);
	const float GetDistanceCasterLight()const { return distanceCasterLight_; }
	//�����W�����Z�b�g
	void SetAtten(const NVec3& atten);
	const NVec3& GetAtten()const { return atten_; }
	//�����p�x���Z�b�g
	void SetFactorAngle(const NVec2& factorAngle);
	const NVec2& GetFactorAngle()const { return factorAngleCos_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};