#pragma once
#include "IModel.h"
#include "NObjModel.h"
#include "NFbxModel.h"
#include <unordered_map>
#include <mutex>

// モデル管理 //

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, std::unique_ptr<IModel>> sModelMap;	//モデル群
	static std::mutex sMtx;	// 排他制御

	// モデルデータコンテナ
	static std::vector<IModel> sModelDatas;
	// 次に使うモデルデータの番号
	static uint32_t sIndexModelData;
	// モデルのあるディレクトリパス
	static std::string sDirectoryPath;

public:
	//全部のモデル読み込み
	static void AllLoad();
	//モデルデータの取得
	static IModel* GetModel(const std::string& modelHandle);

	//objモデル読み込み
	//"modelname" = モデル名
	static IModel* LoadObjModel(const std::string& modelname, const std::string& modelHandle);
	//fbxモデル読み込み
	//"modelname" = モデル名
	static IModel* LoadFbxModel(const std::string& modelname, const std::string& modelHandle);
};