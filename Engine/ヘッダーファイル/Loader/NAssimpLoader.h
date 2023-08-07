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
//    const std::string fileName; //ファイルパス
//    std::vector<NMesh> meshies; //出力先メッシュ配列
//    std::vector<Bone> bones_;	// ボーンの配列
//    bool inversU = false;       //U座標反転フラグ
//    bool inversV = false;       //V座標反転フラグ
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
//	const wchar_t* filename = nullptr;	//ファイルパス
//	std::vector<NMesh>& meshes;			//出力先のメッシュ配列
//	std::vector<Bone> bones;
//	bool inverseU = false;				//U座標を反転させるか
//	bool inverseV = false;				//V座標を反転させるか
//};
//
//class NAssimpLoader final
//{
//public:
//	NVertexBuff<NVertexAssimp> vertexBuff_;
//	NIndexBuff indexBuff_;
//
//	bool Load(const ImportSettings& setting); // モデルをロードする
//
//private:
//	void LoadMesh(NMesh& dst, const aiMesh* src, bool inverseU, bool inverseV);
//	void LoadBone(Bone& dst, const aiMesh* src);
//	void LoadTexture(const wchar_t* filename, NMesh& dst, const aiMaterial* src);
//};