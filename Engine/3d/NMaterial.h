#pragma once
#include "NDX12.h"
#include "NTexture.h"
#include "NConstBuff.h"
#include "NVector2.h"
#include "NVector3.h"

#include<memory>
#include <wrl.h>
template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

class NMaterial
{
private:
	D3D12_HEAP_PROPERTIES heapProp_{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc_{};		//リソース

public:
	std::string name;	//マテリアル
	NVector3 ambient;	//アンビエント影響度
	NVector3 diffuse;	//ディフューズ影響度
	NVector3 specular;	//スペキュラー影響度
	float alpha;		//α値
	std::string textureFilename;	//テクスチャファイル名
	NTexture texture;

public:
	//コンストラクタ
	NMaterial();
	~NMaterial();
};

