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
#include "NMesh.h"

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

//typedef struct ImportSetting
//{
//    const std::string fileName; //�t�@�C���p�X
//    std::vector<NMesh> meshies; //�o�͐惁�b�V���z��
//    std::vector<Bone> bones_;	// �{�[���̔z��
//    bool inversU = false;       //U���W���]�t���O
//    bool inversV = false;       //V���W���]�t���O
//}ImportSetting;
//
//typedef struct NodeAnime
//{
//    std::string name;
//    std::vector<NVec3> position;
//    std::vector<double> positionTime;
//    std::vector<NVec3> rotation;
//    std::vector<double> rotationTime;
//    std::vector<NVec3> scale;
//    std::vector<double> scaleTime;
//}NodeAnime;
//
//struct ImportSettings
//{
//	const wchar_t* filename = nullptr;	//�t�@�C���p�X
//	std::vector<NMesh>& meshes;			//�o�͐�̃��b�V���z��
//	std::vector<Bone> bones;
//	bool inverseU = false;				//U���W�𔽓]�����邩
//	bool inverseV = false;				//V���W�𔽓]�����邩
//};
//
//class NAssimpLoader final
//{
//public:
//	NVertexBuff<NVertexAssimp> vertexBuff_;
//	NIndexBuff indexBuff_;
//
//	bool Load(const ImportSettings& setting); // ���f�������[�h����
//
//private:
//	void LoadMesh(NMesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
//	void LoadBone(Bone& dst, const aiMesh* src);
//	void LoadTexture(const wchar_t* filename, NMesh& dst, const aiMaterial* src);
//};