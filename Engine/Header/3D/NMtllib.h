#pragma once
#include "NMaterial.h"
#include <vector>

// マテリアル読み取るやつ //

struct NMtllib
{
public:
	//マテリアル読み込み
	static NMaterial Load(const std::string& directoryPath, const std::string& filename);
};