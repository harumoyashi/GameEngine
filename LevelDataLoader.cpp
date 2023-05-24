#include "LevelDataLoader.h"
#include <string>
#include <fstream>
#include <assert.h>

LevelDataLoader* LevelDataLoader::GetInstance()
{
	static LevelDataLoader instance;
	return &instance;
}

LevelData* LevelDataLoader::Load(const std::string& directoryPath, const std::string& filename)
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
	LevelData* levelData = new LevelData();

	//"objects"�̃I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"])
	{
		Traversal(object, levelData);

		//ToDo:�I�u�W�F�N�g�������ċN�֐��ɂ܂Ƃ߁A�ċN�Ăяo���Ŏ}�𑖍�����
		/*if (object.contains("children"))
		{
			Traversal(object, levelData);
		}
		else
		{
		}*/
		for (nlohmann::json& c : object["children"])
		{
			Traversal(c, levelData);
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

		////�X�P�[�����O�s��
		//objectData.scaling = objectData.scaling.Scale(scale);

		////��]�s��
		//NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rot.z));
		//NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rot.x));
		//NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rot.y));
		//objectData.rotation *= matZ;	//Z������ɉ�]���Ă���
		//objectData.rotation *= matX;	//X������ɉ�]����
		//objectData.rotation *= matY;	//Y������ɉ�]

		////���s�ړ��s��
		//objectData.translation = objectData.translation.Translation(trans);

		//objectData.matWorld = objectData.matWorld.Identity();	//�P�ʍs����
		//objectData.matWorld *= objectData.scaling;		//���[���h���W�ɃX�P�[�����O�𔽉f
		//objectData.matWorld *= objectData.rotation;		//���[���h���W�ɉ�]�𔽉f
		//objectData.matWorld *= objectData.translation;	//���[���h���W�ɕ��s�ړ��𔽉f
	}
}
