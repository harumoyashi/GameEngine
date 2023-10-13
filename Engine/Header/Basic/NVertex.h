#pragma once
#include <d3d12.h>
#include <vector>
#include "NDX12.h"
#include "NVec2.h"
#include "NVec3.h"
#include "NColor.h"
#include "NUtil.h"

struct NVertex
{
	NVec3 pos = { 0, 0, 0 };

	NVertex(NVec3 pos = { 0, 0, 0 })
		: pos(pos) {}
};

//position,uvの2要素
struct NVertexUV
{
	NVec3 pos = { 0, 0, 0 };
	NVec2 uv = { 0, 0 };

	NVertexUV(NVec3 pos = { 0, 0, 0 }, NVec2 uv = { 0, 0 })
		: pos(pos), uv(uv) {}

	bool operator==(const NVertexUV& a) const;
};

//position,normal,uvの3要素
struct NVertexPNU
{
	NVec3 pos = { 0, 0, 0 };
	NVec3 normal = { 0, 0, 1 };
	NVec2 uv = { 0, 0 };

	NVertexPNU(NVec3 pos = { 0, 0, 0 }, NVec3 normal = { 0, 0, 1 }, NVec2 uv = { 0, 0 })
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
struct NVertexParticle
{
	NVec3 pos = { 0, 0, 0 };
	NVec3 rot = { 0, 0, 0 };
	NColor color = NColor::kWhite;
	float scale = 0.0f;

	NVertexParticle(NVec3 pos = { 0, 0, 0 }, NVec3 rot = { 0, 0, 0 },
		NColor color = NColor::kWhite, float scale = 0.0f)
		: pos(pos), rot(rot), color(color), scale(scale) {}

	bool operator==(const NVertexParticle& a) const;
};

//position,normal,uv,boneIndex,boneWeightの5要素
struct NVertexFbx
{
	NVec3 pos = { 0, 0, 0 };
	NVec3 normal = { 0, 0, 1 };
	NVec2 uv = { 0, 0 };
	uint32_t boneIndex[maxBoneIndices];
	float boneWeight[maxBoneIndices];

	bool operator==(const NVertexFbx& a) const;

	/// <summary>
	/// 法線ベクトルを計算
	/// </summary>
	/// <param name="vertices">計算する頂点群</param>
	/// <param name="indices">インデックスリスト（三角形、全て時計回りであること）</param>
	static void CalcNormalVec(std::vector<NVertexFbx>& vertices, std::vector<uint32_t>& indices);
};