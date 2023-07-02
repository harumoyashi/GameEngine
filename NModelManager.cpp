#include "NModelManager.h"
#include "NMtllib.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;

std::map<ModelHandle, uint32_t> NModelManager::sModelMap{};

std::vector<Model> NModelManager::modelDatas_{};
uint32_t NModelManager::indexModelData_ = 0u;

void NModelManager::AllLoad()
{
	LoadModel("sphere", "sphere");
	LoadModel("Cube", "cube");
	LoadModel("plane", "plane");
	LoadModel("busterSword", "busterSword");
	LoadModel("boss_model", "boss");
	LoadModel("cat", "cat");
}

Model NModelManager::GetModel(const std::string& modelHandle)
{
	uint32_t modelNum = sModelMap[modelHandle];
	return modelDatas_[modelNum];
}

uint32_t NModelManager::LoadModel(const std::string& modelname, const std::string& modelHandle)
{
	uint32_t handle = indexModelData_;

	// 読み込み済みモデルデータを検索
	auto it = std::find_if(modelDatas_.begin(), modelDatas_.end(), [&](const auto& modelData_) {
		return modelData_.name == modelname;
		});
	if (it != modelDatas_.end()) {
		// 読み込み済みモデルデータの要素番号を取得
		handle = static_cast<uint32_t>(std::distance(modelDatas_.begin(), it));
		return handle;
	}

	//ファイルストリーム
	std::ifstream file;
	//.objファイルを開く
	const string filename = modelname + ".obj";	//"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/";	//"Resources/triangle_mat/
	file.open(directoryPath + filename);	//"Resources/triangle_mat/triangle_mat.obj"
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	vector<NVector3>positions;	//頂点座標
	vector<NVector3>normals;	//法線ベクトル
	vector<NVector2>texcoords;	//テクスチャUV
	modelDatas_.emplace_back();
	// 書き込むモデルデータの参照
	Model& modelData = modelDatas_.at(handle);
	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			modelData.material = NMtllib::Load(directoryPath, filename);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			NVector3 position{};
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
			NVector2 texcoord{};
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
			NVector3 normal{};
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
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1行分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データに追加
				NVertexPNU vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelData.vertices.emplace_back(vertex);

				//インデックスデータの追加
				modelData.indices.emplace_back((unsigned short)modelData.indices.size());
			}
		}
	}
	//ファイル閉じる
	file.close();

	//バッファの生成
	modelData.vertexBuff.Init(modelData.vertices);
	modelData.indexBuff.Init(modelData.indices);
	//モデル名を登録して
	modelData.name = modelname;

	//実質的にハンドルを次に進める
	indexModelData_++;

	//モデルマップに登録
	sModelMap.insert(std::make_pair(modelHandle, handle));
	return handle;
}