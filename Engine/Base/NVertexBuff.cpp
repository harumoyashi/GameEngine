#include "NVertexBuff.h"

NVertexBuff::NVertexBuff(NVertex* vertices_, uint32_t size)
{
	Init(vertices_, size);
}

NVertexBuff::NVertexBuff(NVertexPNU* vertices_, uint32_t size)
{
	Init(vertices_, size);
}

NVertexBuff::NVertexBuff(std::vector<NVertex> vertices_)
{
	Init(vertices_);
}

NVertexBuff::NVertexBuff(std::vector<NVertexPNU> vertices_)
{
	Init(vertices_);
}

NVertexBuff::NVertexBuff(NVertexAssimp* vertices_, uint32_t size)
{
	Init(vertices_, size);
}

void NVertexBuff::Init(NVertex* vertices_, uint32_t size)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertex) * size);

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertex* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = size;
}

void NVertexBuff::Init(NVertexPNU* vertices_, uint32_t size)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexPNU) * size);

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexPNU* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = size;
}

void NVertexBuff::Init(std::vector<NVertex> vertices_)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertex) * vertices_.size());

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertex* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = sizeof(NVertex);
}

void NVertexBuff::Init(std::vector<NVertexPNU> vertices_)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexPNU) * vertices_.size());

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexPNU* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = sizeof(NVertexPNU);
}

void NVertexBuff::Init(NVertexAssimp* vertices_, uint32_t size)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexAssimp) * size);

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexAssimp* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = size;
}

void NVertexBuff::Init(std::vector<NVertexAssimp> vertices_)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexAssimp) * vertices_.size());

	//頂点バッファリソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = dataSize;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&buff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexAssimp* vertMap_ = nullptr;
	result = buff->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices_.size(); i++) {
		vertMap_[i] = vertices_[i];
	}
	// 繋がりを解除
	buff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view.BufferLocation = buff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view.StrideInBytes = sizeof(NVertexAssimp);
}