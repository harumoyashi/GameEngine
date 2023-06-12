#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>

class NIndexBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NIndexBuff() {};	//既定のコンストラクタ

	//インデックスの配列とその大きさで頂点バッファを作る
	NIndexBuff(uint32_t* list, const uint32_t size);
	//インデックスの配列とvectorで頂点バッファを作る
	NIndexBuff(const std::vector<uint32_t>& list);

	//インデックスの配列とその大きさで頂点バッファを作る
	void Init(uint32_t* list, const uint32_t size);
	//インデックスの配列とvectorで頂点バッファを作る
	void Init(const std::vector<uint32_t>& list);

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW view_{};
};