#pragma once
#include "json.hpp"
#include "NObj3d.h"
#include "NCamera.h"
#include <unordered_map>

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

	NCamera camera;
	std::list<ObjectData> objects;
	std::unordered_map<ModelName, Model> models;
};

class NLevelDataLoader final
{
private:


public:
	//�V���O���g���C���X�^���X�擾
	static NLevelDataLoader* GetInstance();
	std::unique_ptr<LevelData> Load(const std::string& filename);
	//����
	void Traversal(nlohmann::json& object, LevelData* levelData);

	void TraversalCamera(nlohmann::json& camera, LevelData* levelData);

	//���x���G�f�B�^����o�͂��ꂽ�ǂݍ��ݍσI�u�W�F�N�g��z�u���Ă���
	void SetObject(const LevelData* levelData, std::vector<std::unique_ptr<NObj3d>> &obj);

	//�J������ݒ肷��
	NCamera SetCamera(const LevelData* levelData);

private:
	NLevelDataLoader() = default;
	NLevelDataLoader(const NLevelDataLoader&) = delete;
	~NLevelDataLoader() = default;
	NLevelDataLoader& operator=(const NLevelDataLoader&) = delete;
};

