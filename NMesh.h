#pragma once
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"

struct NMesh final
{
public:
	NVertexBuff<NVertexFbx> vertexBuff;		//���_�o�b�t�@
	NIndexBuff indexBuff;					//�C���f�b�N�X�o�b�t�@

	std::vector<NVertexFbx> vertices;		//���_�Q
	std::vector<uint32_t> indices;			//�C���f�b�N�X�Q
};