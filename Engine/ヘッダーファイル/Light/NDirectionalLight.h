#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVec3.h"
#include "NConstBuff.h"

class NDirectionalLight final
{
private://�ÓI�����o�ϐ�
	NVec3 lightdir_ = { 1,0,0 };	 // ���C�g��������
	NVec3 lightcolor_ = NVec3::one; // ���C�g�F

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

	//���C�g�̕������Z�b�g
	void SetLightDir(const NVec3& lightdir);
	const NVec3& GetLightDir()const { return lightdir_; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVec3& lightcolor);
	const NVec3& GetLightColor()const { return lightcolor_; }
	//�L���t���O���Z�b�g
	void SetActive(bool isActive) { isActive_ = isActive; }
	//�L���t���O���擾
	bool GetActive()const { return isActive_; }
};