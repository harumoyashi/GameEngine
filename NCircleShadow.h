#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"

class NCircleShadow
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		NVector3 dir;
		float pad1;	//�p�f�B���O
		NVector3 casterPos;
		float distanceCasterLight;
		NVector3 atten;
		float pad2;	//�p�f�B���O
		NVector2 factoranglecos;
		unsigned int isActive;	//�L���t���O
		float pad3;	//�p�f�B���O
	};

private://�ÓI�����o�ϐ�
	NVector3 dir = { 1,0,0 };			// ����
	float distanceCasterLight = 100.0f;	// �L���X�^�[�ƃ��C�g�̋���
	NVector3 casterPos = { 0,0,0 };		// �L���X�^�[���W(���[���h)
	NVector3 atten = { 0.5f,0.6f,0 };			// ���������W��
	NVector2 factorAngleCos = { 0.2f,0.5f };	// �����p�x

	//�L���t���O
	bool isActive = false;

public://�����o�֐�
	inline void SetDir(const NVector3& dir) { this->dir = dir.Normalize(); }
	inline const NVector3& GetDir() { return dir; }
	//���W���Z�b�g
	inline void SetCasterPos(const NVector3& casterPos) { this->casterPos = casterPos; }
	inline const NVector3& GetCasterPos() { return casterPos; }
	//�L���X�^�[�ƃ��C�g�̋������Z�b�g
	inline void SetDistanceCasterLight(const float& distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
	inline const float& GetDistanceCasterLight() { return distanceCasterLight; }
	//�����W�����Z�b�g
	inline void SetAtten(const NVector3& atten) { this->atten = atten; }
	inline const NVector3& GetAtten() { return atten; }
	//�����p�x���Z�b�g
	inline void SetFactorAngle(const NVector2& factorAngle) {
		this->factorAngleCos.x = cosf(MathUtil::Degree2Radian(factorAngle.x));
		this->factorAngleCos.y = cosf(MathUtil::Degree2Radian(factorAngle.y));
	}
	inline const NVector2& GetFactorAngle() { return factorAngleCos; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive; }
};