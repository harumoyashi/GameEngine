#pragma once
#include "NDX12.h"
#include "NConstBuffData.h"

#include<memory>
#include <wrl.h>

class NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
	D3D12_RESOURCE_DESC resDesc{};		//リソース

	ConstBufferDataTransform* constMapTransform;	//3D変換行列
	ComPtr<ID3D12Resource> constBuff;		//定数バッファのGPUリソースのポインタ

public:
	void Init();

	//定数バッファへ送信
	void TransferMatrix(NMatrix4 matWorld);

private:
	//ヒープ設定
	void SetHeap();
	//リソース設定
	void SetResource();
	//定数バッファの生成
	void Create();
	//定数バッファのマッピング
	void Mapping();
};