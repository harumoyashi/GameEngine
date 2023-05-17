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
	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

public:
	NConstBuff<ConstBuffDataColor>* cbColor;
	NConstBuff<ConstBuffDataMaterial>* cbMaterial;

	std::string name;	//マテリアル
	NVector3 ambient;	//アンビエント影響度
	NVector3 diffuse;	//ディフューズ影響度
	NVector3 specular;	//スペキュラー影響度
	float alpha;		//α値
	NColor color;		//色
	std::string textureFilename;	//テクスチャファイル名
	NTexture texture;

public:
	//コンストラクタ
	NMaterial();
	~NMaterial();
#pragma region 初期化まわり
	//初期化
	void Init();
	
	//色情報転送
	void TransferColor();
	//光情報転送
	void TransferMaterial();

	//色情報変更(int型0~255)
	void SetColor(int R = 255, int G = 255, int B = 255, int A = 255);
#pragma endregion
#pragma region 描画まわり
	//定数バッファビュー(CBV)の設定コマンド
	void SetCBV();
#pragma endregion
};

