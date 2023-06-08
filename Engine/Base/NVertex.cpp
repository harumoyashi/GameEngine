#include "NVertex.h"
#include <cassert>

bool NVertexPNU::operator==(const NVertexPNU& a) const
{
	if (pos.x != a.pos.x || pos.y != a.pos.y || pos.z != a.pos.z)
	{
		return false;
	}
	else if (normal.x != a.normal.x || normal.y != a.normal.y || normal.z != a.normal.z)
	{
		return false;
	}
	else if (uv.x != a.uv.x || uv.y != a.uv.y)
	{
		return false;
	}

	return true;
}

void NVertexPNU::CalcNormalVec(std::vector<NVertexPNU> vertices, std::vector<uint32_t> indices)
{
	assert(indices.size() % 3 == 0);
	for (uint32_t i = 0; i < indices.size() / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		uint32_t index0 = indices[i * 3 + 0];
		uint32_t index1 = indices[i * 3 + 1];
		uint32_t index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		NVector3 p0 = vertices[index0].pos;
		NVector3 p1 = vertices[index1].pos;
		NVector3 p2 = vertices[index2].pos;

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		NVector3 v1 = p1 - p0;
		NVector3 v2 = p2 - p0;

		//外積は両方から垂直なベクトル
		NVector3 normal = v1.Cross(v2);

		//正規化
		normal = normal.Normalize();

		//求めた法線を頂点データに代入
		vertices[index0].normal = normal;
		vertices[index1].normal = normal;
		vertices[index2].normal = normal;
	}
}

bool NVertexAssimp::operator==(const NVertexAssimp& a) const
{
	if (pos.x != a.pos.x || pos.y != a.pos.y || pos.z != a.pos.z)
	{
		return false;
	}
	else if (normal.x != a.normal.x || normal.y != a.normal.y || normal.z != a.normal.z)
	{
		return false;
	}
	else if (uv.x != a.uv.x || uv.y != a.uv.y)
	{
		return false;
	}
	if (tangent.x != a.tangent.x || tangent.y != a.tangent.y || tangent.z != a.tangent.z)
	{
		return false;
	}
	if (color.r != a.color.r || color.g != a.color.g || color.b != a.color.b || color.a != a.color.a)
	{
		return false;
	}

	return true;
}

void NVertexAssimp::CalcNormalVec(std::vector<NVertexAssimp> vertices, std::vector<uint32_t> indices)
{
	assert(indices.size() % 3 == 0);
	for (uint32_t i = 0; i < indices.size() / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数にいれる
		uint32_t index0 = indices[i * 3 + 0];
		uint32_t index1 = indices[i * 3 + 1];
		uint32_t index2 = indices[i * 3 + 2];

		//三角形を構成する頂点座標をベクトルに代入
		NVector3 p0 = vertices[index0].pos;
		NVector3 p1 = vertices[index1].pos;
		NVector3 p2 = vertices[index2].pos;

		//p0→p1ベクトル、p0→p2ベクトルを計算(ベクトルの減算)
		NVector3 v1 = p1 - p0;
		NVector3 v2 = p2 - p0;

		//外積は両方から垂直なベクトル
		NVector3 normal = v1.Cross(v2);

		//正規化
		normal = normal.Normalize();

		//求めた法線を頂点データに代入
		vertices[index0].normal = normal;
		vertices[index1].normal = normal;
		vertices[index2].normal = normal;
	}
}

bool NVertexUV::operator==(const NVertexUV& a) const
{
	if (pos.x != a.pos.x || pos.y != a.pos.y || pos.z != a.pos.z)
	{
		return false;
	}
	else if (uv.x != a.uv.x || uv.y != a.uv.y)
	{
		return false;
	}

	return true;
}
