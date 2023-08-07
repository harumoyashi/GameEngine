#pragma once
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"

struct NMesh final
{
public:
	NVertexBuff<NVertexPNU> vertexBuff;		//���_�o�b�t�@
	NIndexBuff indexBuff;					//�C���f�b�N�X�o�b�t�@

	std::vector<NVertexPNU> vertices;		//���_�Q
	std::vector<uint32_t> indices;			//�C���f�b�N�X�Q
};