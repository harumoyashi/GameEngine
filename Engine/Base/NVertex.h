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

//position,uvの2要素
class NVertexUV
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector2 uv = { 0, 0 };

	NVertexUV(NVector3 pos = { 0, 0, 0 }, NVector2 uv = { 0, 0 })
		: pos(pos), uv(uv) {}

	bool operator==(const NVertexUV& a) const;
};

//position,normal,uvの3要素
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
	static void CalcNormalVec(std::vector<NVertexPNU>& vertices, std::vector<uint32_t>& indices);
};

//position,scale,colorの3要素
class NVertexParticle
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector3 scale = { 0, 0, 0 };
	NColor color = NColor::kWhite;

	NVertexParticle(NVector3 pos = { 0, 0, 0 }, NVector3 scale = { 0, 0, 0 }, NColor color = NColor::kWhite)
		: pos(pos), scale(scale), color(color) {}

	bool operator==(const NVertexParticle& a) const;
};

//position,normal,uv,boneIndex,boneWeightの5要素
class NVertexAssimp
{
public:
	NVector3 pos = { 0, 0, 0 };
	NVector3 normal = { 0, 0, 1 };
	NVector2 uv = { 0, 0 };
	std::vector<uint32_t> boneIndex{ 4 };
	std::vector<float> boneWeight{4};

	NVertexAssimp(NVector3 pos = { 0, 0, 0 }, NVector3 normal = { 0, 0, 1 }, NVector2 uv = { 0, 0 },
		std::vector<uint32_t> boneIndex = {}, std::vector<float> boneWeight = {})
		: pos(pos), normal(normal), uv(uv), boneIndex(boneIndex), boneWeight(boneWeight) {}

	bool operator==(const NVertexAssimp& a) const;

	/// <summary>
	/// 法線ベクトルを計算
	/// </summary>
	/// <param name="vertices">計算する頂点群</param>
	/// <param name="indices">インデックスリスト（三角形、全て時計回りであること）</param>
	static void CalcNormalVec(std::vector<NVertexAssimp>& vertices, std::vector<uint32_t>& indices);
};