#pragma once
#include "NDX12.h"
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NConstBuff.h"
#include "NMaterial.h"

#include <wrl.h>

class NModel
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	std::string name;
	NVertexBuff vertexBuff;
	NIndexBuff indexBuff;

	std::vector<NVertexPNU> vertices_;
	std::vector<uint32_t> indices;

	NMaterial material;

public:
	void Create(const std::string modelname);

private:
	void LoadObjFile(const std::string modelname);
};