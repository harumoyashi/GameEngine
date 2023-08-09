#include "NModelManager.h"
#include "NMtllib.h"
#include "NAssimpLoader.h"

#include <fstream>
#include <sstream>
#include <string>

std::unordered_map<ModelHandle, uint32_t> NModelManager::sModelMap{};

std::vector<IModel> NModelManager::sModelDatas{};
uint32_t NModelManager::sIndexModelData = 0u;
std::string NModelManager::sDirectoryPath = "Resources/Model/";

void NModelManager::AllLoad()
{
	LoadObjModel("sphere", "sphere");
	LoadObjModel("Cube", "cube");
	LoadObjModel("plane", "plane");
	LoadObjModel("busterSword", "busterSword");
	LoadObjModel("boss_model", "boss");
	LoadObjModel("cat", "cat");
	LoadObjModel("mouse", "mouse");
}

IModel* NModelManager::GetModel(const std::string& modelHandle)
{
	uint32_t modelNum = sModelMap[modelHandle];
	return &sModelDatas[modelNum];
}

uint32_t NModelManager::LoadObjModel(const std::string& modelname, const std::string& modelHandle)
{
	uint32_t handle = sIndexModelData;

	// 読み込み済みモデルデータを検索
	auto it = std::find_if(sModelDatas.begin(), sModelDatas.end(), [&](const auto& modelData_) {
		return modelData_.name == modelname;
		});
	if (it != sModelDatas.end()) {
		// 読み込み済みモデルデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(sModelDatas.begin(), it));
		return handle;
	}

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

	std::vector<NVec3>positions;	//頂点座標
	std::vector<NVec3>normals;	//法線ベクトル
	std::vector<NVec2>texcoords;	//テクスチャUV
	sModelDatas.emplace_back();
	// 書き込むモデルデータの参照
	IModel& modelData = sModelDatas.at(handle);
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
			std::string filename;
			line_stream >> filename;
			//マテリアル読み込み
			modelData.material = NMtllib::Load(directoryPath, filename);
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
				NVertexPNU vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelData.mesh.vertices.emplace_back(vertex);

				//インデックスデータの追加
				modelData.mesh.indices.emplace_back((unsigned short)modelData.mesh.indices.size());
			}
		}
	}
	//ファイル閉じる
	file.close();

	//バッファの生成
	modelData.mesh.vertexBuff.Init(modelData.mesh.vertices);
	modelData.mesh.indexBuff.Init(modelData.mesh.indices);
	//モデル名を登録して
	modelData.name = modelname;

	//実質的にハンドルを次に進める
	sIndexModelData++;

	//モデルマップに登録
	sModelMap.insert(std::make_pair(modelHandle, handle));
	return handle;
}

uint32_t NModelManager::LoadFbxModel(const std::string& modelname, const std::string& modelHandle)
{
	uint32_t handle = sIndexModelData;

	// 読み込み済みモデルデータを検索
	auto it = std::find_if(sModelDatas.begin(), sModelDatas.end(), [&](const auto& modelData_) {
		return modelData_.name == modelname;
		});
	if (it != sModelDatas.end()) {
		// 読み込み済みモデルデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(sModelDatas.begin(), it));
		return handle;
	}

	// 書き込むモデルデータの参照
	IModel& modelData = sModelDatas.at(handle);

	// モデル生成
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = modelname;

	// モデルと同じ名前のフォルダーから読み込む
	std::string path = sDirectoryPath + modelname + "/";
	std::string fbxfile = modelname + ".fbx";
	std::string fullPath = path + fbxfile;

	// assimpでロードする
	NAssimpLoader::GetInstance()->Load(fullPath, model.get());

	// バッファー生成
	model->mesh.vertexBuff.Init(model->mesh.vertices);
	model->mesh.indexBuff.Init(model->mesh.indices);

	//代入
	modelData.material = model->material;
	modelData.mesh = model->mesh;
	modelData.name = model->name;
	modelData.format = model->format;

	//実質的にハンドルを次に進める
	sIndexModelData++;

	//モデルマップに登録
	sModelMap.insert(std::make_pair(modelHandle, handle));
	return handle;
}
