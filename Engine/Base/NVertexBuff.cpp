#include "NVertexBuff.h"

NVertexBuff::NVertexBuff(NVertex* vertices, const uint32_t size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(NVertexPNU* vertices, const uint32_t size)
{
	Init(vertices, size);
}

NVertexBuff::NVertexBuff(const std::vector<NVertex>& vertices)
{
	Init(vertices);
}

NVertexBuff::NVertexBuff(const std::vector<NVertexPNU>& vertices)
{
	Init(vertices);
}

NVertexBuff::NVertexBuff(NVertexAssimp* vertices, const uint32_t size)
{
	Init(vertices, size);
}

void NVertexBuff::Init(NVertex* vertices, const uint32_t size)
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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertex* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(NVertexPNU* vertices, const uint32_t size)
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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexPNU* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(const std::vector<NVertex>& vertices)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertex) * vertices.size());

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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertex* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = sizeof(NVertex);
}

void NVertexBuff::Init(const std::vector<NVertexPNU>& vertices)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexPNU) * vertices.size());

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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexPNU* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = sizeof(NVertexPNU);
}

void NVertexBuff::Init(NVertexAssimp* vertices, const uint32_t size)
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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexAssimp* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < size; i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = size;
}

void NVertexBuff::Init(const std::vector<NVertexAssimp>& vertices)
{
	HRESULT result;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

	uint32_t dataSize = static_cast<uint32_t>(sizeof(NVertexAssimp) * vertices.size());

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
		IID_PPV_ARGS(&buff_));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	NVertexAssimp* vertMap_ = nullptr;
	result = buff_->Map(0, nullptr, (void**)&vertMap_);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (uint32_t i = 0; i < vertices.size(); i++) {
		vertMap_[i] = vertices[i];
	}
	// 繋がりを解除
	buff_->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	// GPU仮想アドレス
	//これ渡すことで、GPU側がどのデータ見ればいいかわかるようになる
	view_.BufferLocation = buff_->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	view_.SizeInBytes = dataSize;
	// 頂点1つ分のデータサイズ
	view_.StrideInBytes = sizeof(NVertexAssimp);
}