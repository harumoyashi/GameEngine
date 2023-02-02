#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"

class NSpotLight
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		NVector3 lightv;
		float pad1;	//�p�f�B���O
		NVector3 lightPos;
		float pad2;	//�p�f�B���O
		NVector3 lightcolor;	//���C�g�̐F
		float pad3;	//�p�f�B���O
		NVector3 lightatten;
		float pad4;	//�p�f�B���O
		NVector2 lightfactoranglecos;
		unsigned int isActive = false;	//�L���t���O
		float pad5;	//�p�f�B���O
	};

private://�ÓI�����o�ϐ�
	NVector3 lightdir = { 1,0,0 };	// ���C�g����
	NVector3 lightpos = { 0,0,0 };		// ���C�g���W(���[���h)
	NVector3 lightcolor = { 1,1,1 };	// ���C�g�F
	NVector3 lightatten = { 1,1,1 };	// ���C�g���������W��
	NVector2 lightFactorAngleCos = { 0.5f,0.2f };	// ���C�g�����p�x

	//�L���t���O
	bool isActive = false;

public://�����o�֐�
	inline void SetLightDir(const NVector3& lightdir) { this->lightdir = lightdir.Normalize(); }
	inline const NVector3& GetLightDir() { return lightdir; }
	//���C�g�̍��W���Z�b�g
	inline void SetLightPos(const NVector3& lightpos) { this->lightpos = lightpos; }
	inline const NVector3& GetLightPos() { return lightpos; }
	//���C�g�̐F���Z�b�g
	inline void SetLightColor(const NVector3& lightcolor) { this->lightcolor = lightcolor; }
	inline const NVector3& GetLightColor() { return lightcolor; }
	//���C�g�̌����W�����Z�b�g
	inline void SetLightAtten(const NVector3& lightatten) { this->lightatten = lightatten; }
	inline const NVector3& GetLightAtten() { return lightatten; }
	//���C�g�̌����p�x���Z�b�g
	inline void SetLightFactorAngle(const NVector2& lightFactorAngle) {
		this->lightFactorAngleCos.x = cosf(MathUtil::Degree2Radian(lightFactorAngle.x));
		this->lightFactorAngleCos.y = cosf(MathUtil::Degree2Radian(lightFactorAngle.y));
	}
	inline const NVector2& GetLightFactorAngle() { return lightFactorAngleCos; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive; }
};