#pragma once
#include "NDX12.h"
#include "NTexture.h"
#include "NConstBuff.h"
#include "NVec2.h"
#include "NVec3.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

struct NMaterial
{
	std::string name;	//�}�e���A��
	NVec3 ambient;	//�A���r�G���g�e���x
	NVec3 diffuse;	//�f�B�t���[�Y�e���x
	NVec3 specular;	//�X�y�L�����[�e���x
	float alpha;		//���l
	std::string textureFilename;	//�e�N�X�`���t�@�C����
	NTexture texture;

	//�R���X�g���N�^
	NMaterial();
	~NMaterial();
};

