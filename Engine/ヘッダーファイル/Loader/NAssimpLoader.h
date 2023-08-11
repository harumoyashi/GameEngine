#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <memory>
#include "NFbxModel.h"

//�O���錾
struct aiMesh;
struct aiScene;
struct aiNode;

class NAssimpLoader final
{
private:
	static const int maxBoneIndices = 4;

private:
	void ParseMesh(FbxModel* model, aiMesh* mesh);								//���b�V���̉��
	void ParseVertex(FbxModel* model, aiMesh* mesh);							//���_�̉��
	void ParseFace(FbxModel* model, aiMesh* mesh);								//�t�F�C�X�̉��
	void ParseSkin(FbxModel* model, aiMesh* mesh);								//�X�L���̉��
	void ParseMaterial(FbxModel* model, const aiScene* scene);					//�}�e���A���̉��
	void ParseNodeRecursive(FbxModel* model, Node* parent,const aiNode* node);	//���

public:
	//�V���O���g���C���X�^���X�擾
	static NAssimpLoader* GetInstance();
	bool Load(const std::string& filePath, FbxModel* model);	//���f�������[�h����
	//Assimp��Mat4������Mat4�^�ɕϊ�
	static NMatrix4 AssimpMatToMat4(const aiMatrix4x4& mat);
	//�t�@�C�����؂����ĕԂ�
	std::string ExractFileName(const std::string& path);

private:
	NAssimpLoader() = default;
};