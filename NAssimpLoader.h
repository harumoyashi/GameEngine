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

	//�R���X�g���N�^
	Bone(const std::string& name)
	{
		this->name = name;
	}
};

typedef struct ImportSetting
{
    const std::string fileName;//�t�@�C���p�X
    std::vector<Mesh> meshies; //�o�͐惁�b�V���z��
    bool inversU = false; //U���W���]�t���O
    bool inversV = false; //V���W���]�t���O
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
    std::vector<std::unique_ptr<Mesh>> meshes; //�o�͐惁�b�V���z��
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
	const wchar_t* filename = nullptr;	//�t�@�C���p�X
	std::vector<Mesh>& meshes;			//�o�͐�̃��b�V���z��
	//std::vector<Bone> bones;
	bool inverseU = false;				//U���W�𔽓]�����邩
	bool inverseV = false;				//V���W�𔽓]�����邩
};

class NAssimpLoader
{
public:
	NVertexBuff vertexBuff_;
	NIndexBuff indexBuff_;

	bool Load(const ImportSettings& setting); // ���f�������[�h����

private:
	void LoadMesh(Mesh& dst, const aiMesh* src, const bool inverseU, const bool inverseV);
	void LoadBone(Bone& dst, const aiMesh* src);
	void LoadTexture(const wchar_t* filename, Mesh& dst, const aiMaterial* src);
};