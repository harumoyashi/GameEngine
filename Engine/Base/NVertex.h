#pragma once
#include <d3d12.h>
#include <vector>
#include "NDX12.h"
#include "NVector2.h"
#include "NVector3.h"
#include "NColor.h"

class NVertex
{
public:
	NVector3 pos = { 0, 0, 0 };

	NVertex(NVector3 pos = { 0, 0, 0 })
		: pos(pos) {}
};

class NVertexUV
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector2 uv = { 0, 0 };

	NVertexUV(NVector3 pos = { 0, 0, 0 }, NVector2 uv = { 0, 0 })
		: pos(pos), uv(uv) {}

	bool operator==(const NVertexUV& a) const;
};

class NVertexPNU
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector3 normal = { 0, 0, 1 };
	NVector2 uv = { 0, 0 };

	NVertexPNU(NVector3 pos = { 0, 0, 0 }, NVector3 normal = { 0, 0, 1 }, NVector2 uv = { 0, 0 })
		: pos(pos), normal(normal), uv(uv) {}

	bool operator==(const NVertexPNU& a) const;

	/// <summary>
	/// 法線ベクトルを計算
	/// </summary>
	/// <param name="vertices">計算する頂点群</param>
	/// <param name="indices">インデックスリスト（三角形、全て時計回りであること）</param>
	static void CalcNormalVec(std::vector<NVertexPNU> vertices, std::vector<unsigned int> indices);
};

class NVertexAssimp
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector3 normal = { 0, 0, 1 };
	NVector2 uv = { 0, 0 };
	NVector3 tangent = { 0, 0, 1 };	//接空間
	NColor color = { 255,255,255,255 };

	NVertexAssimp(NVector3 pos = { 0, 0, 0 }, NVector3 normal = { 0, 0, 1 }, NVector2 uv = { 0, 0 },
		NVector3 tangent = { 0,0,1 }, NColor color = { 255,255,255,255 })
		: pos(pos), normal(normal), uv(uv),tangent(tangent),color(color) {}

	bool operator==(const NVertexAssimp& a) const;

	/// <summary>
	/// 法線ベクトルを計算
	/// </summary>
	/// <param name="vertices">計算する頂点群</param>
	/// <param name="indices">インデックスリスト（三角形、全て時計回りであること）</param>
	static void CalcNormalVec(std::vector<NVertexAssimp> vertices, std::vector<unsigned int> indices);
};