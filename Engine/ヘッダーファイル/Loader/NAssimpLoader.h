#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include "NFbxModel.h"

//前方宣言
struct aiMesh;
struct aiScene;
struct aiNode;

class NAssimpLoader final
{
private:
	static const int maxBoneIndices = 4;

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);								//メッシュの解析
	void ParseVertex(FbxModel* model, aiMesh* mesh);							//頂点の解析
	void ParseFace(FbxModel* model, aiMesh* mesh);								//フェイスの解析
	void ParseSkin(FbxModel* model, aiMesh* mesh);								//スキンの解析
	void ParseMaterial(FbxModel* model, const aiScene* scene);					//マテリアルの解析
	void ParseNodeRecursive(FbxModel* model, Node* parent,const aiNode* node);	//解析

public:
	//シングルトンインスタンス取得
	static NAssimpLoader* GetInstance();
	bool Load(const std::string& filePath, FbxModel* model);	//モデルをロードする
	//AssimpのMat4を自作Mat4型に変換
	static NMatrix4 AssimpMatToMat4(const aiMatrix4x4& mat);
	//ファイル名切り取って返す
	std::string ExractFileName(const std::string& path);

private:
	NAssimpLoader() = default;
};