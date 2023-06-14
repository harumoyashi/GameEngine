#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"
#include "NConstBuff.h"

class NDirectionalLight
{
private://�ÓI�����o�ϐ�
	NVector3 lightdir_ = { 1,0,0 };	 // ���C�g��������
	NVector3 lightcolor_ = { 1,1,1 }; // ���C�g�F

	//�_�[�e�B�t���O
	bool isDirty_ = false;
	//�L���t���O
	bool isActive_ = false;

public:
	//�萔�o�b�t�@
	std::unique_ptr<NConstBuff<ConstBuffDataLight>> cbLight;

public: //�����o�֐�
	NDirectionalLight();
	~NDirectionalLight();

	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(const uint32_t rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//���C�g�̕������Z�b�g
	void SetLightDir(const NVector3& lightdir);
	inline NVector3 GetLightDir() { return lightdir_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor_; }
	//�L���t���O���Z�b�g
	inline void SetActive(const bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive_; }
};