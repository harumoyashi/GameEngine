#pragma once
#define NONINMAX
#include <d3d12.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include "NVertexBuff.h"
#include "NIndexBuff.h"
#include "NMatrix4.h"

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

struct Mesh
{
	std::vector<NVertexPNU> vertices;
	std::vector<uint32_t> indices;

	std::wstring textureName;
};
struct Vertex;
struct aiMesh;
struct aiMaterial;

struct Bone
{
	std::string name;

	NMatrix4 offsetMatrix;
	NMatrix4 finalMatrix;

	//コンストラクタ
	Bone(const std::string& name)
	{
		this->name = name;
	}
};

typedef struct ImportSetting
{
    const std::string fileName;//ファイルパス
    std::vector<Mesh> meshies; //出力先メッシュ配列
    bool inversU = false; //U座標反転フラグ
    bool inversV = false; //V座標反転フラグ
}ImportSetting;

typedef struct NodeAnime
{
    std::string name;
    std::vector<NVector3> position;
    std::vector<double> positionTime;
    std::vector<NVector3> rotation;
    std::vector<double> rotationTime;
    std::vector<NVector3> scale;
    std::vector<double> scaleTime;
}NodeAnime;

typedef struct Animation
{
    std::string name;
    double duration;
    double ticksPerSecond;
    std::vector<NodeAnime> channels;
}Animation;

typedef struct Node
{
    std::string name;
    std::vector<std::unique_ptr<Mesh>> meshes; //出力先メッシュ配列
    NVector3 scale = { 1,1,1 };
    NVector3 rotation = { 0,0,0 };
    NVector3 translation = { 0,0,0 };
    NMatrix4 transform;
    NMatrix4 globalTransform;
    NMatrix4 globalInverseTransform;
    NMatrix4 AnimaetionParentMat;
    Node* parent = nullptr;

}Node;

struct ImportSettings
{
	const wchar_t* filename = nullptr;	//ファイルパス
	std::vector<Mesh>& meshes;			//出力先のメッシュ配列
	//std::vector<Bone> bones;
	bool inverseU = false;				//U座標を反転させるか
	bool inverseV = false;				//V座標を反転させるか
};

class NAssimpLoader
{
public:
	NVertexBuff vertexBuff_;
	NIndexBuff indexBuff_;

	bool Load(const ImportSettings& setting); // モデルをロードする

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, const bool inverseU, const bool inverseV);
	void LoadBone(Bone& dst, const aiMesh* src);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};