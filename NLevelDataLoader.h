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
	//�V���O���g���C���X�^���X�擾
	static NLevelDataLoader* GetInstance();
	std::unique_ptr<LevelData> Load(const std::string& directoryPath, const std::string& filename);
	//����
	void Traversal(nlohmann::json& object, LevelData* levelData);

	//���x���G�f�B�^����o�͂��ꂽ�ǂݍ��ݍσI�u�W�F�N�g��z�u���Ă���
	void SetObject(const LevelData* levelData, std::vector<std::unique_ptr<NObj3d>> &obj);
};

