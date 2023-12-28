#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include "NFbxModel.h"

// Assimpの読み込み //

//前方宣言
struct aiMesh;
struct aiScene;
struct aiNode;

class NAssimpLoader final
{
private:
	//メッシュの解析
	void ParseMesh(FbxModel* model, aiMesh* mesh);
	//頂点の解析
	void ParseVertex(FbxModel* model, aiMesh* mesh);
	//フェイスの解析
	void ParseFace(FbxModel* model, aiMesh* mesh);
	//スキンの解析
	void ParseSkin(FbxModel* model, aiMesh* mesh);

public:
	//マテリアルの解析
	void ParseMaterial(FbxModel* model, const aiScene* scene);
	//解析
	void ParseNodeRecursive(FbxModel* model, Node* parent, const aiNode* node);

public:
	//シングルトンインスタンス取得
	static NAssimpLoader* GetInstance();
	//モデルをロードする
	bool Load(const std::string& filePath, FbxModel* model);
	//AssimpのMat4を自作Mat4型に変換
	static NMatrix4 AssimpMatToMat4(const aiMatrix4x4& mat);
	//ファイル名切り取って返す
	std::string ExractFileName(const std::string& path);

private:
	//コンストラクタ
	NAssimpLoader() = default;
};