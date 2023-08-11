#pragma once
#include "NVec2.h"
#include "NMatrix4.h"
#include "NColor.h"

//�萔�o�b�t�@�p�f�[�^�\����//

//3D�ϊ��s��
struct  ConstBuffDataTransform
{
	//NMatrix4 mat;	//3D�ϊ��s��
	NMatrix4 viewproj;	// �r���[�v���W�F�N�V�����s��
	NMatrix4 world;		// ���[���h�s��
	NVec3 cameraPos;	// �J�������W(���[���h���W)
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
	NVec3 ambient;	// �A���r�G���g�W��
	float pad1;
	NVec3 diffuse;	// �f�B�t���[�Y�W��
	float pad2;
	NVec3 specular;	// �X�y�L�����[�W��
	float pad3;
	float alpha;	// �A���t�@
};

//���s����
struct ConstBuffDataLight
{
	NVec3 dir;
	float pad1;
	NVec3 color;
	uint32_t active;
};

//�|�C���g���C�g
struct ConstBuffDataPointLight
{
	NVec3 pos;
	float pad1;
	NVec3 color;
	float pad2;
	NVec3 atten;
	uint32_t active;
};

//�X�|�b�g���C�g
struct ConstBuffDataSpotLight
{
	NVec3 dir;
	float pad1;
	NVec3 pos;
	float pad2;
	NVec3 color;
	float pad3;
	NVec3 atten;
	float pad4;
	NVec2 factorAngleCos;
	uint32_t active;
	float pad5;
};

//�ۉe
struct ConstBuffDataCircleShadow
{
	NVec3 dir;
	float pad1;
	NVec3 pos;
	float disCasterLight;
	NVec3 atten;
	float pad2;
	NVec2 factorAngleCos;
	uint32_t active;
	float pad3;
};

//�^�C��
struct ConstBuffDataTile
{
	float divide;		//������
	float activityArea;	//�s���͈�
};