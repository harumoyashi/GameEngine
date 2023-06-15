#pragma once
#include "json.hpp"
#include <NObj3d.h>

typedef std::string ModelName;

struct LevelData
{
	struct ObjectData
	{
		std::string filename;
		NVector3 trans;
		NVector3 rot;
		NVector3 scale;
	};

	std::list<ObjectData> objects;
	std::map<ModelName, NModel> models;
};

class NLevelDataLoader
{
private:


public:
	//シングルトンインスタンス取得
	static NLevelDataLoader* GetInstance();
	std::unique_ptr<LevelData> Load(const std::string& directoryPath, const std::string& filename);
	//走査
	void Traversal(nlohmann::json& object, LevelData* levelData);

	//レベルエディタから出力された読み込み済オブジェクトを配置していく
	void SetObject(const LevelData* levelData, std::vector<std::unique_ptr<NObj3d>> &obj);
};

