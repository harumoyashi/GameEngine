#pragma once
#include "NDX12.h"
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"
#include "NMaterial.h"

struct Model
{
	std::string name;						//モデル名
	NVertexBuff<NVertexPNU> vertexBuff;		//頂点バッファ
	NIndexBuff indexBuff;					//インデックスバッファ

	std::vector<NVertexPNU> vertices;		//頂点群
	std::vector<uint32_t> indices;			//インデックス群

	NMaterial material;						//マテリアル
};

class NModel
{
public:
	void Init();
	void Finalize();
};