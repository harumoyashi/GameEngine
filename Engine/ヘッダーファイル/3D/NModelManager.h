#pragma once
#include "IModel.h"
#include "NObjModel.h"
#include "NFbxModel.h"
#include <unordered_map>
#include <mutex>

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, std::unique_ptr<IModel>> sModelMap;	//���f���Q
	static std::mutex sMtx;	// �r������

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
	static IModel* LoadObjModel(const std::string& modelname, const std::string& modelHandle);
	//fbx���f���ǂݍ���
	//"modelname" = ���f����
	static IModel* LoadFbxModel(const std::string& modelname, const std::string& modelHandle);
};