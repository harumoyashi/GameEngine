#include "LevelDataLoader.h"
#include <string>
#include <fstream>
#include <assert.h>

LevelDataLoader* LevelDataLoader::GetInstance()
{
	static LevelDataLoader instance;
	return &instance;
}

std::unique_ptr<LevelData> LevelDataLoader::Load(const std::string& directoryPath, const std::string& filename)
{
	//�A�����ăt���p�X������
	const std::string fullpath = directoryPath + filename;

	//�t�@�C���X�g���[��
	std::ifstream file;

	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	//JSON�����񂩂�𓀂����f�[�^
	nlohmann::json deserialized;

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();

	//"objects"�̃I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		Traversal(object, levelData.get());

		//�ċN�Ăяo���Ŏ}�𑖍�����
		for (nlohmann::json& c : object["children"])
		{
			Traversal(c, levelData.get());
		}
	}
	return levelData;
}

void LevelDataLoader::Traversal(nlohmann::json& object, LevelData* levelData)
{
	assert(object.contains("type"));

	//��ʂ��擾
	std::string type = object["type"].get<std::string>();

	//��ނ��Ƃ̏���
	//MESH
	if (type.compare("MESH") == 0)
	{
		//�v�f�ǉ�
		levelData->objects.emplace_back(LevelData::ObjectData{});
		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name"))
		{
			//�t�@�C����
			objectData.filename = object["file_name"];
		}

		//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json& transform = object["transform"];

		//��UVec3�ɒu��������
		objectData.trans = { (float)transform["translation"][1],(float)transform["translation"][2],-(float)transform["translation"][0] };
		objectData.rot = { -(float)transform["rotation"][1],-(float)transform["rotation"][2],(float)transform["rotation"][0] };
		objectData.scale = { (float)transform["scaling"][1],(float)transform["scaling"][2],(float)transform["scaling"][0] };
	}
}
