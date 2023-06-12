#include "NIndexBuff.h"
#include "NDX12.h"

NIndexBuff::NIndexBuff(uint32_t* indices, uint32_t size)
{
	Init(indices, size);
}

NIndexBuff::NIndexBuff(std::vector<uint32_t> indices)
{
	Init(indices);
}

void NIndexBuff::Init(uint32_t* indices, uint32_t size)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<UINT>(sizeof(UINT) * size);

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	//インデックスバッファをマッピング
	uint32_t* indexMap = nullptr;
	result = buff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全インデックスに対して
	//for (uint32_t i = 0; i < size; i++)
	//{
	//	indexMap[i] = indices_[i];	//インデックスをコピー
	//}
	memcpy(indexMap, indices, size);
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// GPU仮想アドレス
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	//インデックス1個分のサイズ
	view_.Format = DXGI_FORMAT_R32_UINT;
	// インデックスバッファのサイズ
	view_.SizeInBytes = dataSize;
}

void NIndexBuff::Init(std::vector<uint32_t> indices)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = (uint32_t)(sizeof(uint32_t) * indices.size());

	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (uint64_t)dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// インデックスバッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	//インデックスバッファをマッピング
	uint32_t* indexMap = nullptr;
	result = buff_->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// 全インデックスに対して
	for (uint32_t i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// GPU仮想アドレス
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	//インデックス1個分のサイズ
	view_.Format = DXGI_FORMAT_R32_UINT;
	// インデックスバッファのサイズ
	view_.SizeInBytes = dataSize;
}
