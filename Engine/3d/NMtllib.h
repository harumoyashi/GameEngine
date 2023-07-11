#pragma once
#include "NMaterial.h"
#include <vector>

class NMtllib
{
public:
	static NMaterial Load(const std::string& directoryPath, const std::string& filename);
};