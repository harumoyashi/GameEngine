#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "NVertex.h"

class NVertexBuff
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	NVertexBuff() {};	//既定のコンストラクタ

	//Vertex(Pos)の配列とその大きさで頂点バッファを作る
	NVertexBuff(NVertex* vertices, const uint32_t size);
	//Vertex(Pos)のvectorで頂点バッファを作る
	NVertexBuff(const std::vector<NVertex>& vertices);

	//Vertex(PosNormalUv)の配列とその大きさで頂点バッファを作る
	NVertexBuff(NVertexPNU* vertices, const uint32_t size);
	//Vertex(PosNormalUv)のvectorで頂点バッファを作る
	NVertexBuff(const std::vector<NVertexPNU>& vertices);

	//Vertex(NVertexAssimp)の配列とその大きさで頂点バッファを作る
	NVertexBuff(NVertexAssimp* vertices, const uint32_t size);
	//Vertex(NVertexAssimp)のvectorで頂点バッファを作る
	NVertexBuff(const std::vector<NVertexAssimp>& vertices);

	//Vertex(Pos)の配列とその大きさで頂点バッファを作る
	void Init(NVertex* vertices, const uint32_t size);
	//Vertex(Pos)のvectorで頂点バッファを作る
	void Init(const std::vector<NVertex>& vertices);

	//Vertex(PosNormalUv)の配列とその大きさで頂点バッファを作る
	void Init(NVertexPNU* vertices, const uint32_t size);
	//Vertex(PosNormalUv)のvectorで頂点バッファを作る
	void Init(const std::vector<NVertexPNU>& vertices);

	//Vertex(assimp用)の配列とその大きさで頂点バッファを作る
	void Init(NVertexAssimp* vertices, const uint32_t size);
	//Vertex(assimp用)のvectorで頂点バッファを作る
	void Init(const std::vector<NVertexAssimp>& vertices);

	ComPtr<ID3D12Resource> buff_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view_{};
};