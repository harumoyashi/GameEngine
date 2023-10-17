#pragma once
#include "NMaterial.h"
#include <vector>

struct NMtllib
{
public:
	static NMaterial Load(const std::string& directoryPath, const std::string& filename);
};