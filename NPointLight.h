#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <assert.h>
#include <wrl.h>
#include "NVector3.h"

class NPointLight
{
public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		NVector3 lightPos;
		float pad1;	//�p�f�B���O
		NVector3 lightcolor;	//���C�g�̐F
		float pad2;	//�p�f�B���O
		NVector3 lightatten;
		unsigned int isActive = false;	//�L���t���O
	};

private://�ÓI�����o�ϐ�
	NVector3 lightpos = { 0,0,0 };		// ���C�g���W(���[���h)
	NVector3 lightcolor = { 1,1,1 };	// ���C�g�F
	NVector3 lightatten = { 0.2f,0.2f,0.2f };	// ���C�g���������W��

	//�L���t���O
	bool isActive = false;

public://�����o�֐�
	//���C�g�̍��W���Z�b�g
	inline void SetLightPos(const NVector3& lightpos) { this->lightpos = lightpos; }
	inline NVector3 GetLightPos() { return lightpos; }
	//���C�g�̐F���Z�b�g
	inline void SetLightColor(const NVector3& lightcolor) { this->lightcolor = lightcolor; }
	inline NVector3 GetLightColor() { return lightcolor; }
	//���C�g�̌����W�����Z�b�g
	inline void SetLightAtten(const NVector3& lightatten) { this->lightatten = lightatten; }
	inline NVector3 GetLightAtten() { return lightatten; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool isActive) { this->isActive = isActive; }
	//�L���t���O���擾
	inline bool GetActive() { return isActive; }
};