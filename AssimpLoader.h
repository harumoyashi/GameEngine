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
	const wchar_t* filename = nullptr;	//�t�@�C���p�X
	std::vector<Mesh>& meshes;			//�o�͐�̃��b�V���z��
	bool inverseU = false;				//U���W�𔽓]�����邩
	bool inverseV = false;				//V���W�𔽓]�����邩
};

class AssimpLoader
{
public:
	NVertexBuff vertexBuff_;
	NIndexBuff indexBuff_;

	bool Load(ImportSettings setting); // ���f�������[�h����

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};