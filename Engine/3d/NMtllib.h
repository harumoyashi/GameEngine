#pragma once
#include "NMaterial.h"
#include <vector>

class NMtllib
{
public:
	static NMaterial Load(const std::string& dir_ectoryPath, const std::string& filename);
};