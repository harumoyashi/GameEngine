#pragma once
#include "IModel.h"
#include "NObjModel.h"
#include "NFbxModel.h"
#include <unordered_map>

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, uint32_t> sModelMap;	//モデル群

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
	static uint32_t LoadObjModel(const std::string& modelname, const std::string& modelHandle);
	//fbxモデル読み込み
	//"modelname" = モデル名
	static uint32_t LoadFbxModel(const std::string& modelname, const std::string& modelHandle);
};