#pragma once
#include "NAssimpLoader.h"
#include "NConstBuff.h"
#include "NMaterial.h"
#include "NLightGroup.h"
#include <memory>

class NAssimpModel final
{
private:
	std::vector<Mesh> meshes_;	// ���b�V���̔z��
	//std::vector<Bone> bones_;	// �{�[���̔z��
	std::vector<NVertexBuff<NVertexPNU>> vertexBuffers_; // ���b�V���̐����̒��_�o�b�t�@
	std::vector<NIndexBuff> indexBuffers_; // ���b�V���̐����̃C���f�b�N�X�o�b�t�@
	const wchar_t* kModelFile;

	//���邩�킩��񂯂ǉ���
	NMatrix4 matWorld_;		//3D�ϊ��s��
	NMaterial material_;	//�}�e���A��
	NColor color_;			//�F

	//���C�g����
	static NLightGroup* sLightGroup;

	ImportSettings importSetting_ = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	{
		kModelFile,
		meshes_,
		//bones_,
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
	NVector3 scale_ = { 1.0f,1.0f,1.0f };		//�X�P�[�����O�{��
	NVector3 rotation_ = { 0.0f,0.0f,0.0f };	//��]�p
	NVector3 position_ = { 0.0f,0.0f,0.0f };	//���W

	//�{�[���C���f�b�N�X�̍ő吔
	static const uint32_t MAX_BONE_INDICES = 4;

public:
	//�t�@�C�����烁�b�V������ǂݎ��A����ɉ������o�b�t�@�𐶐�����
	void Load(const wchar_t* filename);
	void Init();
	void Update();
	void Draw();

	inline static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
};