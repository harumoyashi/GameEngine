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
	std::string name_;
	NVertexBuff vertexBuff_;
	NIndexBuff indexBuff_;

	std::vector<NVertexPNU> vertices_;
	std::vector<uint32_t> indices_;

	NMaterial material_;

public:
	void Create(const std::string& modelname);

private:
	void LoadObjFile(const std::string& modelname);
};