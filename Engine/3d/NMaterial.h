#pragma once
#include "NDX12.h"
#include "NTexture.h"
#include "NConstBuff.h"
#include "NVector2.h"
#include "NVector3.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

struct NMaterial
{
	std::string name;	//マテリアル
	NVector3 ambient;	//アンビエント影響度
	NVector3 diffuse;	//ディフューズ影響度
	NVector3 specular;	//スペキュラー影響度
	float alpha;		//α値
	std::string textureFilename;	//テクスチャファイル名
	NTexture texture;

	//コンストラクタ
	NMaterial();
	~NMaterial();
};

