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
	NConstBuff<ConstBuffDataPointLight>* cbPointLight;

private://�ÓI�����o�ϐ�
	NVector3 lightpos = { 0,0,0 };		// ���C�g���W(���[���h)
	NVector3 lightcolor = { 1,1,1 };	// ���C�g�F
	NVector3 lightatten = { 0.2f,0.2f,0.2f };	// ���C�g���������W��

	//�_�[�e�B�t���O
	bool isDirty = false;
	//�L���t���O
	bool isActive = false;

public://�����o�֐�
	NPointLight();
	~NPointLight();

	//������
	void Initialize();
	//�X�V
	void Update();
	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(UINT rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//���C�g�̍��W���Z�b�g
	void SetLightPos(const NVector3& lightpos);
	inline NVector3 GetLightPos() { return lightpos; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor; }
	//���C�g�̌����W�����Z�b�g
	void SetLightAtten(const NVector3& lightatten);
	inline NVector3 GetLightAtten() { return lightatten; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive; }
};