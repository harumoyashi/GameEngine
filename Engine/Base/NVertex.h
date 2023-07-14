#pragma once
#include <d3d12.h>
#include <vector>
#include "NDX12.h"
#include "NVector2.h"
#include "NVector3.h"
#include "NColor.h"

struct NVertex
{
	NVector3 pos = { 0, 0, 0 };

	NVertex(NVector3 pos = { 0, 0, 0 })
		: pos(pos) {}
};

//position,uv��2�v�f
struct NVertexUV
{
	NVector3 pos = { 0, 0, 0 };
	NVector2 uv = { 0, 0 };

	NVertexUV(NVector3 pos = { 0, 0, 0 }, NVector2 uv = { 0, 0 })
		: pos(pos), uv(uv) {}

	bool operator==(const NVertexUV& a) const;
};

//position,normal,uv��3�v�f
struct NVertexPNU
{
	NVector3 pos = { 0, 0, 0 };
	NVector3 normal = { 0, 0, 1 };
	NVector2 uv = { 0, 0 };

	NVertexPNU(NVector3 pos = { 0, 0, 0 }, NVector3 normal = { 0, 0, 1 }, NVector2 uv = { 0, 0 })
		: pos(pos), normal(normal), uv(uv) {}

	bool operator==(const NVertexPNU& a) const;

	/// <summary>
	/// �@���x�N�g�����v�Z
	/// </summary>
	/// <param name="vertices">�v�Z���钸�_�Q</param>
	/// <param name="indices">�C���f�b�N�X���X�g�i�O�p�`�A�S�Ď��v���ł��邱�Ɓj</param>
	static void CalcNormalVec(std::vector<NVertexPNU>& vertices, std::vector<uint32_t>& indices);
};

//position,scale,color��3�v�f
struct NVertexParticle
{
	NVector3 pos = { 0, 0, 0 };
	NVector3 rot = { 0, 0, 0 };
	NColor color = NColor::kWhite;
	float scale = 0.0f;

	NVertexParticle(NVector3 pos = { 0, 0, 0 }, NVector3 rot = { 0, 0, 0 },
		NColor color = NColor::kWhite, float scale = 0.0f)
		: pos(pos), rot(rot), color(color), scale(scale) {}

	bool operator==(const NVertexParticle& a) const;
};

//position,normal,uv,boneIndex,boneWeight��5�v�f
struct NVertexAssimp
{
	NVector3 pos = { 0, 0, 0 };
	NVector3 normal = { 0, 0, 1 };
	NVector2 uv = { 0, 0 };
	std::vector<uint32_t> boneIndex{ 4 };
	std::vector<float> boneWeight{ 4 };

	NVertexAssimp(NVector3 pos = { 0, 0, 0 }, NVector3 normal = { 0, 0, 1 }, NVector2 uv = { 0, 0 },
		std::vector<uint32_t> boneIndex = {}, std::vector<float> boneWeight = {})
		: pos(pos), normal(normal), uv(uv), boneIndex(boneIndex), boneWeight(boneWeight) {}

	bool operator==(const NVertexAssimp& a) const;

	/// <summary>
	/// �@���x�N�g�����v�Z
	/// </summary>
	/// <param name="vertices">�v�Z���钸�_�Q</param>
	/// <param name="indices">�C���f�b�N�X���X�g�i�O�p�`�A�S�Ď��v���ł��邱�Ɓj</param>
	static void CalcNormalVec(std::vector<NVertexAssimp>& vertices, std::vector<uint32_t>& indices);
};