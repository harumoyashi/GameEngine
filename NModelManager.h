#pragma once
#include "NModel.h"
#include <unordered_map>

typedef std::string ModelHandle;

class NModelManager final
{
private:
	static std::unordered_map<ModelHandle, uint32_t> sModelMap;	//���f���Q

	// ���f���f�[�^�R���e�i
	static std::vector<Model> modelDatas_;
	// ���Ɏg�����f���f�[�^�̔ԍ�
	static uint32_t indexModelData_;

public:
	//�S���̃��f���ǂݍ���
	static void AllLoad();
	//���f���f�[�^�̎擾
	static Model GetModel(const std::string& modelHandle);
	
	//���f���ǂݍ���
	//"modelname" = ���f����
	static uint32_t LoadModel(const std::string& modelname, const std::string& modelHandle);
};