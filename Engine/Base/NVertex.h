#pragma once
#include <d3d12.h>
#include <vector>
#include "NDX12.h"
#include "NVector2.h"
#include "NVector3.h"

class NVertex
{
public:
	NVector3 pos = { 0, 0, 0 };

	NVertex(NVector3 pos = { 0, 0, 0 })
		: pos(pos) {}
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
	/// �@���x�N�g�����v�Z
	/// </summary>
	/// <param name="vertices">�v�Z���钸�_�Q</param>
	/// <param name="indices">�C���f�b�N�X���X�g�i�O�p�`�A�S�Ď��v���ł��邱�Ɓj</param>
	static void CalcNormalVec(std::vector<NVertexPNU> vertices, std::vector<unsigned int> indices);
};