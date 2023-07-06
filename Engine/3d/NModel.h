#pragma once
#include "NDX12.h"
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"
#include "NMaterial.h"

struct Model
{
	std::string name;						//���f����
	NVertexBuff<NVertexPNU> vertexBuff;		//���_�o�b�t�@
	NIndexBuff indexBuff;					//�C���f�b�N�X�o�b�t�@

	std::vector<NVertexPNU> vertices;		//���_�Q
	std::vector<uint32_t> indices;			//�C���f�b�N�X�Q

	NMaterial material;						//�}�e���A��
};

class NModel
{
public:
	void Init();
	void Finalize();
};