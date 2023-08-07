#pragma once
#include "NMesh.h"
#include "NMaterial.h"
#include "NUtil.h"

struct IModel
{
	std::string name;	//���f����
	NMesh mesh;			//���b�V��
	NMaterial material;	//�}�e���A��
	ModelFormat format;	//���f���̎��

	virtual ~IModel() = default;
};