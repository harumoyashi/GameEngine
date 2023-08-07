#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NVec2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NSpotLight final
{
public:
	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataSpotLight>> cbSpotLight;

private://�ÓI�����o�ϐ�
	NVec3 lightdir_ = { 0,-1,0 };	// ���C�g����
	NVec3 lightpos_ = { 0,10,0 };	// ���C�g���W(���[���h)
	NVec3 lightcolor_ = NVec3::one;	// ���C�g�F
	NVec3 lightatten_ = NVec3::zero;	// ���C�g���������W��
	NVec2 lightFactorAngleCos_ = { 0.5f,0.2f };	// ���C�g�����p�x

	//�_�[�e�B�t���O
	bool isDirty_ = false;
	//�L���t���O
	bool isActive_ = false;

public://�����o�֐�
	NSpotLight();
	~NSpotLight();

	//������
	void Init();

	void SetLightDir(const NVec3& lightdir);
	const NVec3& GetLightDir()const { return lightdir_; }
	//���C�g�̍��W���Z�b�g
	void SetLightPos(const NVec3& lightpos);
	const NVec3& GetLightPos()const { return lightpos_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVec3& lightcolor);
	const NVec3& GetLightColor()const { return lightcolor_; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVec3& lightatten);
	const NVec3& GetLightAtten()const { return lightatten_; }
	//���C�g�̌����p�x���Z�b�g
	void SetLightFactorAngle(const NVec2& lightFactorAngle);
	const NVec2& GetLightFactorAngle()const { return lightFactorAngleCos_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};