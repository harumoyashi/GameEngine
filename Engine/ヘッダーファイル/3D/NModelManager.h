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
	static std::vector<IModel> modelDatas_;
	// ���Ɏg�����f���f�[�^�̔ԍ�
	static uint32_t indexModelData_;

public:
	//�S���̃��f���ǂݍ���
	static void AllLoad();
	//���f���f�[�^�̎擾
	static IModel* GetModel(const std::string& modelHandle);
	
	//���f���ǂݍ���
	//"modelname" = ���f����
	static uint32_t LoadObjModel(const std::string& modelname, const std::string& modelHandle);
	static uint32_t LoadFbxModel(const std::string& modelname, const std::string& modelHandle);
};