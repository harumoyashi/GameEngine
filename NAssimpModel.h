#pragma once
#include "NAssimpLoader.h"
#include "NConstBuff.h"
#include "NMaterial.h"
#include <memory>

class NAssimpModel
{
private:
	std::vector<Mesh> meshes_; // ���b�V���̔z��
	std::vector<NVertexBuff> vertexBuffers_; // ���b�V���̐����̒��_�o�b�t�@
	std::vector<NIndexBuff> indexBuffers_; // ���b�V���̐����̃C���f�b�N�X�o�b�t�@
	const wchar_t* kModelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";


	//���邩�킩��񂯂ǉ���
	NMatrix4 matWorld_;		//3D�ϊ��s��
	NMaterial material_;	//�}�e���A��
	NColor color_;			//�F

	ImportSettings importSetting_ = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	{
		kModelFile,
		meshes_,
		false,
		true // �A���V�A�̃��f���́A�e�N�X�`����UV��V�������]���Ă���ۂ��H�̂œǂݍ��ݎ���UV���W���t�]������
	};

	//assimp�̃��[�_�[
	NAssimpLoader loader_;

	//�萔�o�b�t�@�ǂ�
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;
	std::unique_ptr<NConstBuff<ConstBuffDataMaterial>> cbMaterial_;
	std::unique_ptr<NConstBuff<ConstBuffDataColor>> cbColor_;

public:
	//�t�@�C�����烁�b�V������ǂݎ��A����ɉ������o�b�t�@�𐶐�����
	void Load();
	void Init();
	void Update();
	void Draw();
};