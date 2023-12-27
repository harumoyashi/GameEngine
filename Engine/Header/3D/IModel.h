#pragma once
#include "NMesh.h"
#include "NMaterial.h"
#include "NUtil.h"

// モデルのひな形 //

struct IModel
{
	std::string name;	//モデル名
	NMesh mesh;			//メッシュ
	NMaterial material;	//マテリアル
	ModelFormat format;	//モデルの種類

	virtual ~IModel() = default;
};