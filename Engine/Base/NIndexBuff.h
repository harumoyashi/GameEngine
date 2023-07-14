#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

struct NIndexBuff
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW view_{};

public:
	NIndexBuff() {};	//既定のコンストラクタ

	//インデックスの配列とその大きさで頂点バッファを作る
	NIndexBuff(uint32_t* list, const uint32_t size);
	//インデックスの配列とvectorで頂点バッファを作る
	NIndexBuff(const std::vector<uint32_t>& list);

	//インデックスの配列とその大きさで頂点バッファを作る
	void Init(uint32_t* list, const uint32_t size);
	//インデックスの配列とvectorで頂点バッファを作る
	void Init(const std::vector<uint32_t>& list);

	// ゲッター //
	//バッファー取得
	inline ID3D12Resource* GetBuffer() { return buff_.Get(); }
	//ビュー取得
	inline D3D12_INDEX_BUFFER_VIEW* GetView() { return &view_; }
};