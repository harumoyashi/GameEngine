#pragma once
#include "NDX12.h"
#include "NTexture.h"
#include "NConstBuff.h"
#include "NVector2.h"
#include "NVector3.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class NMaterial
{
private:
	D3D12_HEAP_PROPERTIES heapProp_{};	//�q�[�v
	D3D12_RESOURCE_DESC resDesc_{};		//���\�[�X

public:
	std::string name;	//�}�e���A��
	NVector3 ambient;	//�A���r�G���g�e���x
	NVector3 diffuse;	//�f�B�t���[�Y�e���x
	NVector3 specular;	//�X�y�L�����[�e���x
	float alpha;		//���l
	std::string textureFilename;	//�e�N�X�`���t�@�C����
	NTexture texture;

public:
	//�R���X�g���N�^
	NMaterial();
	~NMaterial();
};

