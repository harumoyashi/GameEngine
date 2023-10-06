#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "NModelManager.h"
#include "NMtllib.h"
#include "NAssimpLoader.h"

#include <fstream>
#include <sstream>
#include <string>

std::unordered_map<ModelHandle, std::unique_ptr<IModel>> NModelManager::sModelMap{};

std::vector<IModel> NModelManager::sModelDatas{};
uint32_t NModelManager::sIndexModelData = 0u;
std::string NModelManager::sDirectoryPath = "Resources/Model/";

void NModelManager::AllLoad()
{
	LoadObjModel("sphere", "sphere");
	LoadObjModel("sphere", "sphere");
	LoadObjModel("Cube", "cube");
	LoadObjModel("plane", "plane");
	LoadObjModel("busterSword", "busterSword");
	LoadObjModel("boss_model", "boss");
	LoadObjModel("catObj", "cat");
	LoadObjModel("mouse", "mouse");
	LoadFbxModel("boneTest", "boneTest");
	LoadFbxModel("Tripping", "girl");
	LoadFbxModel("cat", "catWalk");
}

IModel* NModelManager::GetModel(const std::string& modelHandle)
{
	//uint32_t modelNum = sModelMap[modelHandle];
	return sModelMap[modelHandle].get();
}

IModel* NModelManager::LoadObjModel(const std::string& modelname, const std::string& modelHandle)
{
	//事前に検索かけて、読み込み済みならそのモデルを返す
	for (auto& modelMap : sModelMap)
	{
		if (modelMap.first.c_str() == modelHandle)
		{
			return sModelMap[modelHandle].get();
		}
	}

	uint32_t handle = sIndexModelData;

	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = sDirectoryPath + modelname + "/";
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	// インスタンス生成
	std::unique_ptr<IModel> model = std::make_unique<ObjModel>();

	std::vector<NVec3>positions;	//頂点座標
	std::vector<NVec3>normals;		//法線ベクトル
	std::vector<NVec2>texcoords;	//テクスチャUV

	//1行ずつ読み込む
	std::string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			std::string filenameMat;
			line_stream >> filenameMat;
			//マテリアル読み込み
			model->material = NMtllib::Load(directoryPath, filenameMat);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			NVec3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			NVec2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向転換
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			NVec3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//テクスチャ座標データに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン
		if (key == "f")
		{
			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1行分の文字列をストリームに変換して解析しやすくする
				std::stringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データに追加
				NVertexFbx vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model->mesh.vertices.emplace_back(vertex);

				//インデックスデータの追加
				model->mesh.indices.emplace_back((unsigned short)model->mesh.indices.size());
			}
		}
	}
	//ファイル閉じる
	file.close();

	//バッファの生成
	model->mesh.vertexBuff.Init(model->mesh.vertices);
	model->mesh.indexBuff.Init(model->mesh.indices);
	//モデル名を登録して
	model->name = modelname;

	//mapに格納
	sModelMap.insert(std::make_pair(modelHandle, std::move(model)));

	return sModelMap[modelHandle].get();
}

IModel* NModelManager::LoadFbxModel(const std::string& modelname, const std::string& modelHandle)
{
	// モデル生成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = modelname;

	// モデルと同じ名前のフォルダーから読み込む
	std::string path = sDirectoryPath + modelname + "/";
	std::string fbxfile = modelname + ".fbx";
	std::string fullPath = path + fbxfile;

	// フラグ
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// シーンの読み込み
	model->scene = model->importer.ReadFile(fullPath, flags);

	if (model->scene == nullptr)
	{
		assert(0 && "モデルの読み込みが失敗しました");
	}

	// マテリアルの解析
	NAssimpLoader::GetInstance()->ParseMaterial(model.get(), model->scene);
	NAssimpLoader::GetInstance()->ParseNodeRecursive(model.get(), nullptr, model->scene->mRootNode);

	// バッファー生成
	model->mesh.vertexBuff.Init(model->mesh.vertices);
	model->mesh.indexBuff.Init(model->mesh.indices);

	// mapに格納
	sModelMap.insert(std::make_pair(modelHandle, std::move(model)));

	return sModelMap[modelHandle].get();
}
