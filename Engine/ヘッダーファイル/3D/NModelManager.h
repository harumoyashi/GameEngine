#pragma once
#include "NModel.h"
#include <unordered_map>

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, uint32_t> sModelMap;	//モデル群

	// モデルデータコンテナ
	static std::vector<Model> modelDatas_;
	// 次に使うモデルデータの番号
	static uint32_t indexModelData_;

public:
	//全部のモデル読み込み
	static void AllLoad();
	//モデルデータの取得
	static Model GetModel(const std::string& modelHandle);
	
	//モデル読み込み
	//"modelname" = モデル名
	static uint32_t LoadModel(const std::string& modelname, const std::string& modelHandle);
};