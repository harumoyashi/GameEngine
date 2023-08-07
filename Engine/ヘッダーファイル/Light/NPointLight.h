#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NConstBuff.h"

class NPointLight final
{
public:
	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataPointLight>> cbPointLight;

private://�ÓI�����o�ϐ�
	NVec3 lightpos_ = NVec3::zero;		// ���C�g���W(���[���h)
	NVec3 lightcolor_ = NVec3::one;	// ���C�g�F
	NVec3 lightatten_ = { 0.2f,0.2f,0.2f };	// ���C�g���������W��

	//�_�[�e�B�t���O
	bool isDirty_ = false;
	//�L���t���O
	bool isActive_ = false;

public://�����o�֐�
	NPointLight();
	~NPointLight();

	//������
	void Init();

	//���C�g�̍��W���Z�b�g
	void SetLightPos(const NVec3& lightpos);
	const NVec3& GetLightPos()const { return lightpos_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVec3& lightcolor);
	const NVec3& GetLightColor()const { return lightcolor_; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVec3& lightatten);
	const NVec3& GetLightAtten()const { return lightatten_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};