#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NPointLight final
{
public:
	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataPointLight>> cbPointLight;

private://�ÓI�����o�ϐ�
	NVector3 lightpos_ = { 0,0,0 };		// ���C�g���W(���[���h)
	NVector3 lightcolor_ = { 1,1,1 };	// ���C�g�F
	NVector3 lightatten_ = { 0.2f,0.2f,0.2f };	// ���C�g���������W��

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
	void SetLightPos(const NVector3& lightpos);
	const NVector3& GetLightPos()const { return lightpos_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	const NVector3& GetLightColor()const { return lightcolor_; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVector3& lightatten);
	const NVector3& GetLightAtten()const { return lightatten_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};