#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NPointLight
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
	//�X�V
	void Update();
	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(const uint32_t rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//���C�g�̍��W���Z�b�g
	void SetLightPos(const NVector3& lightpos);
	inline NVector3 GetLightPos() { return lightpos_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor_; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVector3& lightatten);
	inline NVector3 GetLightAtten() { return lightatten_; }
	//�L���t���O���Z�b�g
	inline void SetActive(const bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive_; }
};