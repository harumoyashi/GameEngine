#pragma once
#include "NDX12.h"
#include "NMatrix4.h"

#include<memory>
#include <wrl.h>

//定数バッファ用データ構造体（3D変換行列）
struct  ConstBufferDataTransform
{
	//NMatrix4 mat;	//3D変換行列
	NMatrix4 viewproj;	// ビュープロジェクション行列
	NMatrix4 world;		// ワールド行列
	NVector3 cameraPos;	// カメラ座標(ワールド座標)
};

class NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

	ConstBufferDataTransform* constMapTransform;	//3D変換行列
	ComPtr<ID3D12Resource> constBuff;		//定数バッファのGPUリソースのポインタ

public:
	//ヒープ設定
	void SetHeap();
	//リソース設定
	void SetResource();
	//定数バッファの生成
	void Create();
	//定数バッファのマッピング
	void Mapping();

	//定数バッファへ送信
	void TransferMatrix(NMatrix4 matWorld);
};