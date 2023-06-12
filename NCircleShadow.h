#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NCircleShadow
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
	void Initialize();
	//�X�V
	void Update();
	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(const uint32_t rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//�e�̕������Z�b�g
	void SetDir(const NVector3& dir);
	inline const NVector3& GetDir() { return dir_; }
	//���W���Z�b�g
	void SetCasterPos(const NVector3& casterPos);
	inline const NVector3& GetCasterPos() { return casterPos_; }
	//�L���X�^�[�ƃ��C�g�̋������Z�b�g
	void SetDistanceCasterLight(const float distanceCasterLight);
	inline const float& GetDistanceCasterLight() { return distanceCasterLight_; }
	//�����W�����Z�b�g
	void SetAtten(const NVector3& atten);
	inline const NVector3& GetAtten() { return atten_; }
	//�����p�x���Z�b�g
	void SetFactorAngle(const NVector2& factorAngle);
	inline const NVector2& GetFactorAngle() { return factorAngleCos_; }
	//�L���t���O���Z�b�g
	inline void SetActive(const bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive_; }
};