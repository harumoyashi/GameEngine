#pragma once
#include "NDX12.h"
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"
#include "NMaterial.h"

struct Model
{
	std::string name;
	NVertexBuff vertexBuff;
	NIndexBuff indexBuff;

	std::vector<NVertexPNU> vertices;
	std::vector<uint32_t> indices;

	NMaterial material;
};

class NModel
{
public:
	void Init();
	void Finalize();
};