#pragma once
#include "NVector2.h"
#include "NMatrix4.h"
#include "NColor.h"

//�萔�o�b�t�@�p�f�[�^�\����//

//3D�ϊ��s��
struct  ConstBufferDataTransform
{
	//NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 viewproj;	// �r���[�v���W�F�N�V�����s��
	NMatrix4 world;		// ���[���h�s��
	NVector3 cameraPos;	// �J�������W(���[���h���W)
};

//2D�ϊ��s��
struct ConstantBufferDataTransform2D
{
	NMatrix4 mat;	//3D�ϊ��s��
};

//�r���{�[�h�ϊ��s��
struct ConstantBufferDataBillboard
{
	NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 matBillboard;	//�r���{�[�h�s��
};

//�F
struct ConstantBufferDataColor
{
	NColor color;	//�F
};

//ADS
struct ConstantBufferDataMaterial
{
	NVector3 ambient;	// �A���r�G���g�W��
	float pad1;
	NVector3 diffuse;	// �f�B�t���[�Y�W��
	float pad2;
	NVector3 specular;	// �X�y�L�����[�W��
	float alpha;	// �A���t�@
};

//���C�g
struct ConstantBufferDataLight
{
	NVector3 dir;
	float pad1;
	NVector3 color;
};

//�|�C���g���C�g
struct ConstantBufferDataPointLight
{
	NVector3 pos;
	float pad1;
	NVector3 color;
	float pad2;
	NVector3 atten;
	unsigned int active;
};

//�X�|�b�g���C�g
struct ConstantBufferDataSpotLight
{
	NVector3 vec;
	float pad1;
	NVector3 pos;
	float pad2;
	NVector3 color;
	float pad3;
	NVector3 atten;
	float pad4;
	NVector2 factorAngleCos;
	unsigned int isActive;
	float pad5;
};

//�ۉe
struct ConstantBufferDataCircleShadow
{
	NVector3 vec;
	float pad1;
	NVector3 pos;
	float disCasterLight;
	NVector3 atten;
	float pad2;
	NVector2 factorAngleCos;
	float darkness;
	unsigned int isActive;
};