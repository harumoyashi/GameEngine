#pragma once
#include "IModel.h"
#include "NObjModel.h"
#include "NFbxModel.h"
#include <unordered_map>

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, uint32_t> sModelMap;	//���f���Q

	// ���f���f�[�^�R���e�i
	static std::vector<IModel> sModelDatas;
	// ���Ɏg�����f���f�[�^�̔ԍ�
	static uint32_t sIndexModelData;
	// ���f���̂���f�B���N�g���p�X
	static std::string sDirectoryPath;

public:
	//�S���̃��f���ǂݍ���
	static void AllLoad();
	//���f���f�[�^�̎擾
	static IModel* GetModel(const std::string& modelHandle);
	
	//obj���f���ǂݍ���
	//"modelname" = ���f����
	static uint32_t LoadObjModel(const std::string& modelname, const std::string& modelHandle);
	//fbx���f���ǂݍ���
	//"modelname" = ���f����
	static uint32_t LoadFbxModel(const std::string& modelname, const std::string& modelHandle);
};