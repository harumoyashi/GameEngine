#pragma once
#include "NVector2.h"
#include "NMatrix4.h"
#include "NColor.h"

//�萔�o�b�t�@�p�f�[�^�\����//

//3D�ϊ��s��
struct  ConstBuffDataTransform
{
	//NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 viewproj;	// �r���[�v���W�F�N�V�����s��
	NMatrix4 world;		// ���[���h�s��
	NVector3 cameraPos;	// �J�������W(���[���h���W)
};

//2D�ϊ��s��
struct ConstBuffDataTransform2D
{
	NMatrix4 mat;	//3D�ϊ��s��
};

//�r���{�[�h�ϊ��s��
struct ConstBuffDataBillboard
{
	NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 matBillboard;	//�r���{�[�h�s��
};

//�F
struct ConstBuffDataColor
{
	NColor color;	//�F
};

//ADS
struct ConstBuffDataMaterial
{
	NVector3 ambient;	// �A���r�G���g�W��
	float pad1;
	NVector3 diffuse;	// �f�B�t���[�Y�W��
	float pad2;
	NVector3 specular;	// �X�y�L�����[�W��
	float pad3;
	float alpha;	// �A���t�@
};

//���s����
struct ConstBuffDataLight
{
	NVector3 dir;
	float pad1;
	NVector3 color;
	uint32_t active;
};

//�|�C���g���C�g
struct ConstBuffDataPointLight
{
	NVector3 pos;
	float pad1;
	NVector3 color;
	float pad2;
	NVector3 atten;
	uint32_t active;
};

//�X�|�b�g���C�g
struct ConstBuffDataSpotLight
{
	NVector3 dir;
	float pad1;
	NVector3 pos;
	float pad2;
	NVector3 color;
	float pad3;
	NVector3 atten;
	float pad4;
	NVector2 factorAngleCos;
	uint32_t active;
	float pad5;
};

//�ۉe
struct ConstBuffDataCircleShadow
{
	NVector3 dir;
	float pad1;
	NVector3 pos;
	float disCasterLight;
	NVector3 atten;
	float pad2;
	NVector2 factorAngleCos;
	uint32_t active;
	float pad3;
};

//�^�C��
struct ConstBuffDataTile
{
	float divide;		//������
	float activityArea;	//�s���͈�
};