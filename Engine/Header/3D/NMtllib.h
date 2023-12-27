#pragma once
#include "NMaterial.h"
#include <vector>

// マテリアル読み取るやつ //

struct NMtllib
{
public:
	static NMaterial Load(const std::string& directoryPath, const std::string& filename);
};