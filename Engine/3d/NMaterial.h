#pragma once
#include "NDX12.h"
#include "NTexture.h"

#include "NVector2.h"
#include "NVector3.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

//�萔�o�b�t�@�p�\����
struct ConstBufferDataMaterial
{
	NVector3 ambient;	//�A���r�G���g�W��
	float pad1;			//�p�f�B���O(���Ԗ���)
	NVector3 diffuse;	//�f�B�t���[�Y�W��
	float pad2;			//�p�f�B���O
	NVector3 specular;	//�X�y�L�����[�W��
	float alpha;		//���l
	NVector3 color;		//�F
};

class NMaterial
{
private:
	D3D12_HEAP_PROPERTIES heapProp{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc{};		//���\�[�X

public:
	ComPtr<ID3D12Resource> constBuffMaterial;
	ConstBufferDataMaterial* constMapMaterial;		//�F(RGBA)

	std::string name;	//�}�e���A��
	NVector3 ambient;	//�A���r�G���g�e���x
	NVector3 diffuse;	//�f�B�t���[�Y�e���x
	NVector3 specular;	//�X�y�L�����[�e���x
	float alpha;		//���l
	NVector3 color;		//�F
	std::string textureFilename;	//�e�N�X�`���t�@�C����
	NTexture texture;

public:
	//�R���X�g���N�^
	NMaterial();
	~NMaterial();
#pragma region �������܂��
	//������
	void Init();
	//�q�[�v�ݒ�
	void SetHeap();
	//���\�[�X�ݒ�
	void SetResource();
	//�萔�o�b�t�@�̐���
	void CreateCB();
	//�萔�o�b�t�@�̃}�b�s���O
	void MappingCB();
	//�����]��
	void TransferLight();
	//�F���]��
	void TransferColor();
	//�F���ύX(int�^0~255)
	void SetColor(int R = 255, int G = 255, int B = 255, int A = 255);
#pragma endregion
#pragma region �`��܂��
	//�萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	void SetCBV();
#pragma endregion
};

