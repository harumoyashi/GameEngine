#pragma once
#define NONINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include "NVertexBuff.h"
#include "NIndexBuff.h"

struct Mesh
{
	std::vector<NVertexAssimp> vertices;
	std::vector<uint32_t> indices;

	std::wstring textureName;
};
struct Vertex;
struct aiMesh;
struct aiMaterial;

struct ImportSettings
{
	const wchar_t* filename = nullptr;	//ファイルパス
	std::vector<Mesh>& meshes;			//出力先のメッシュ配列
	bool inverseU = false;				//U座標を反転させるか
	bool inverseV = false;				//V座標を反転させるか
};

class AssimpLoader
{
public:
	NVertexBuff vertexBuff_;
	NIndexBuff indexBuff_;

	bool Load(ImportSettings setting); // モデルをロードする

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};