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
		NMatrix4 matWorld;
	};

	std::list<ObjectData> objects;
	std::map<ModelName, NModel> models;
};

class LevelDataLoader
{
private:


public:
	//シングルトンインスタンス取得
	static LevelDataLoader* GetInstance();
	LevelData* Load(const std::string& directoryPath, const std::string& filename);
	//走査
	void Traversal(nlohmann::json& object, LevelData* levelData);


};

