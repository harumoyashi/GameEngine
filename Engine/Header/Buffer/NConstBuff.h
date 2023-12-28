#pragma once
#include "NDX12.h"
#include "NConstBuffData.h"

#include<memory>
#include <wrl.h>

// 定数バッファ //

template<typename T>
struct NConstBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> constBuff_;		//定数バッファのGPUリソースのポインタ
	T* constMap_;	// マッピング用

public:
	//コンストラクタ
	NConstBuff() :constMap_(nullptr) {}
	//デストラクタ
	~NConstBuff() = default;

	//初期化
	void Init()
	{
		HRESULT result;
		//ヒープ設定
		D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPUへの転送用

		//リソース設定
		D3D12_RESOURCE_DESC resDesc{};		//リソース
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = (sizeof(T) + 0xff) & ~0xff;	//256バイトアラインメント
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		//定数バッファの生成
		result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
			&heapProp,	//ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc,	//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuff_)
		);
		assert(SUCCEEDED(result));

		//定数バッファのマッピング
		result = constBuff_->Map(0, nullptr, (void**)&constMap_);	//マッピング
		assert(SUCCEEDED(result));

		constBuff_->Unmap(0, nullptr);
	};

	//解除
	void Unmap() { constBuff_->Unmap(0, nullptr); }
};