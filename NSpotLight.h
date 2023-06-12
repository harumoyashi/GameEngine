#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NVector2.h"
#include "NMathUtil.h"
#include "NConstBuff.h"

class NSpotLight
{
public:
	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataSpotLight>> cbSpotLight;

private://�ÓI�����o�ϐ�
	NVector3 lightdir_ = { 1,0,0 };	// ���C�g����
	NVector3 lightpos_ = { 0,0,0 };		// ���C�g���W(���[���h)
	NVector3 lightcolor_ = { 1,1,1 };	// ���C�g�F
	NVector3 lightatten_ = { 1,1,1 };	// ���C�g���������W��
	NVector2 lightFactorAngleCos_ = { 0.5f,0.2f };	// ���C�g�����p�x

	//�_�[�e�B�t���O
	bool isDirty_ = false;
	//�L���t���O
	bool isActive_ = false;

public://�����o�֐�
	NSpotLight();
	~NSpotLight();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(uint32_t rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	void SetLightDir(const NVector3& lightdir);
	inline const NVector3& GetLightDir() { return lightdir_; }
	//���C�g�̍��W���Z�b�g
	void SetLightPos(const NVector3& lightpos);
	inline const NVector3& GetLightPos() { return lightpos_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	inline const NVector3& GetLightColor() { return lightcolor_; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVector3& lightatten);
	inline const NVector3& GetLightAtten() { return lightatten_; }
	//���C�g�̌����p�x���Z�b�g
	void SetLightFactorAngle(const NVector2& lightFactorAngle);
	inline const NVector2& GetLightFactorAngle() { return lightFactorAngleCos_; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive_; }
};