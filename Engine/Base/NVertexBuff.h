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
	NVertexBuff(NVertex* vertices, uint32_t size);
	//Vertex(Pos)のvectorで頂点バッファを作る
	NVertexBuff(std::vector<NVertex> vertices);

	//Vertex(PosNormalUv)の配列とその大きさで頂点バッファを作る
	NVertexBuff(NVertexPNU* vertices, uint32_t size);
	//Vertex(PosNormalUv)のvectorで頂点バッファを作る
	NVertexBuff(std::vector<NVertexPNU> vertices);

	//Vertex(NVertexAssimp)の配列とその大きさで頂点バッファを作る
	NVertexBuff(NVertexAssimp* vertices, uint32_t size);
	//Vertex(NVertexAssimp)のvectorで頂点バッファを作る
	NVertexBuff(std::vector<NVertexAssimp> vertices);

	//Vertex(Pos)の配列とその大きさで頂点バッファを作る
	void Init(NVertex* vertices, uint32_t size);
	//Vertex(Pos)のvectorで頂点バッファを作る
	void Init(std::vector<NVertex> vertices);

	//Vertex(PosNormalUv)の配列とその大きさで頂点バッファを作る
	void Init(NVertexPNU* vertices, uint32_t size);
	//Vertex(PosNormalUv)のvectorで頂点バッファを作る
	void Init(std::vector<NVertexPNU> vertices);

	//Vertex(assimp用)の配列とその大きさで頂点バッファを作る
	void Init(NVertexAssimp* vertices, uint32_t size);
	//Vertex(assimp用)のvectorで頂点バッファを作る
	void Init(std::vector<NVertexAssimp> vertices);

	ComPtr<ID3D12Resource> buff = nullptr;
	D3D12_VERTEX_BUFFER_VIEW view{};
};