#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"

class NDirectionalLight
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		NVector3 lightv;		//���C�g�ւ̕�����\���x�N�g��
		float pad1;		//�p�f�B���O
		NVector3 lightcolor;	//���C�g�̐F
		bool isActive = false;	//�L���t���O
	};

private://�ÓI�����o�ϐ�
	NVector3 lightdir = { 1,0,0 };	 // ���C�g��������
	NVector3 lightcolor = { 1,1,1 }; // ���C�g�F

	//�_�[�e�B�t���O
	bool isDirty = false;
	//�L���t���O
	bool isActive = false;

public://�ÓI�����o�֐�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	static NDirectionalLight* Creare();

public: //�����o�֐�

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	//rootParameterIndex = ���[�g�p�����[�^�̉��ԖڂɃZ�b�g���邩
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	//�萔�o�b�t�@�]��
	void TransferConstBuffer();

	//���C�g�̕������Z�b�g
	void SetLightDir(const NVector3& lightdir);
	inline NVector3 GetLightDir() { return lightdir; }
	//���C�g�̐F���Z�b�g
	void SetLightColor(const NVector3& lightcolor);
	inline NVector3 GetLightColor() { return lightcolor; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive; }
};