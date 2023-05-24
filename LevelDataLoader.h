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
	//�V���O���g���C���X�^���X�擾
	static LevelDataLoader* GetInstance();
	LevelData* Load(const std::string& directoryPath, const std::string& filename);
	//����
	void Traversal(nlohmann::json& object, LevelData* levelData);


};

