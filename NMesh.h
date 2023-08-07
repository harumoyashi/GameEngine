#pragma once
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"

struct NMesh final
{
public:
	NVertexBuff<NVertexPNU> vertexBuff;		//頂点バッファ
	NIndexBuff indexBuff;					//インデックスバッファ

	std::vector<NVertexPNU> vertices;		//頂点群
	std::vector<uint32_t> indices;			//インデックス群
};