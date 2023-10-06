#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "NVertex.h"

template<typename T> class NVertexBuff final
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view_{};
	T* vertMap_;		//マッピング用

public:
	NVertexBuff() {};	//既定のコンストラクタ

	//頂点のポインタとサイズで初期化
	void Init(T* vertices, const uint32_t size)
	{
		HRESULT result;

		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

		uint32_t dataSize = static_cast<uint32_t>(sizeof(T) * size);

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
		vertMap_ = nullptr;
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
	//頂点の配列で初期化
	void Init(const std::vector<T>& vertices)
	{
		HRESULT result;

		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; //GPUへの転送用

		uint32_t dataSize = static_cast<unsigned int>(sizeof(T) * vertices.size());

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
		vertMap_ = nullptr;
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
		view_.StrideInBytes = sizeof(vertices[0]);
	}

	//頂点情報の転送
	void TransferBuffer(const std::vector<T>& vertices)
	{
		// 全頂点に対してコピー
		copy(vertices.begin(), vertices.end(), vertMap_);
	}

	// ゲッター //
	//バッファー取得
	ID3D12Resource* GetBuffer() { return buff_.Get(); }
	//ビュー取得
	D3D12_VERTEX_BUFFER_VIEW* GetView() { return &view_; }
};