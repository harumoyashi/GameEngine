#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "NModelManager.h"
#include "NMtllib.h"
#include "NAssimpLoader.h"

#include <fstream>
#include <sstream>
#include <string>

std::unordered_map<ModelHandle, std::unique_ptr<IModel>> NModelManager::sModelMap{};

std::vector<IModel> NModelManager::sModelDatas{};
uint32_t NModelManager::sIndexModelData = 0u;
std::string NModelManager::sDirectoryPath = "Resources/Model/";

void NModelManager::AllLoad()
{
	LoadObjModel("sphere", "sphere");
	LoadObjModel("Cube", "cube");
	LoadObjModel("plane", "plane");
	LoadObjModel("busterSword", "busterSword");
	LoadObjModel("boss_model", "boss");
	LoadObjModel("cat", "cat");
	LoadObjModel("mouse", "mouse");
	LoadFbxModel("boneTest", "boneTest");
	LoadFbxModel("Tripping", "girl");
}

IModel* NModelManager::GetModel(const std::string& modelHandle)
{
	//uint32_t modelNum = sModelMap[modelHandle];
	return sModelMap[modelHandle].get();
}

IModel* NModelManager::LoadObjModel(const std::string& modelname, const std::string& modelHandle)
{
	uint32_t handle = sIndexModelData;

	//// �ǂݍ��ݍς݃��f���f�[�^������
	//auto it = std::find_if(sModelDatas.begin(), sModelDatas.end(), [&](const auto& model->) {
	//	return model->.name == modelname;
	//	});
	//if (it != sModelDatas.end()) {
	//	// �ǂݍ��ݍς݃��f���f�[�^�̗v�f�ԍ����擾
	//	handle = static_cast<uint32_t>(std::distance(sModelDatas.begin(), it));
	//	return handle;
	//}

	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	const std::string filename = modelname + ".obj";
	const std::string directoryPath = sDirectoryPath + modelname + "/";
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	// �C���X�^���X����
	std::unique_ptr<IModel> model = std::make_unique<ObjModel>();

	std::vector<NVec3>positions;	//���_���W
	std::vector<NVec3>normals;	//�@���x�N�g��
	std::vector<NVec2>texcoords;	//�e�N�X�`��UV

	//1�s���ǂݍ���
	std::string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		std::string key;
		getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			std::string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			model->material = NMtllib::Load(directoryPath, filename);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			NVec3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
		}

		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			NVec2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�����]��
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}

		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			NVec3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}

		//�擪������f�Ȃ�|���S��
		if (key == "f")
		{
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			std::string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::stringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexNormal;

				//���_�f�[�^�ɒǉ�
				NVertexFbx vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model->mesh.vertices.emplace_back(vertex);

				//�C���f�b�N�X�f�[�^�̒ǉ�
				model->mesh.indices.emplace_back((unsigned short)model->mesh.indices.size());
			}
		}
	}
	//�t�@�C������
	file.close();

	//�o�b�t�@�̐���
	model->mesh.vertexBuff.Init(model->mesh.vertices);
	model->mesh.indexBuff.Init(model->mesh.indices);
	//���f������o�^����
	model->name = modelname;

	//map�Ɋi�[
	sModelMap.insert(std::make_pair(modelHandle, std::move(model)));

	return sModelMap[modelHandle].get();
}

IModel* NModelManager::LoadFbxModel(const std::string& modelname, const std::string& modelHandle)
{
	// ���f������
	std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
	model->name = modelname;

	// ���f���Ɠ������O�̃t�H���_�[����ǂݍ���
	std::string path = sDirectoryPath + modelname + "/";
	std::string fbxfile = modelname + ".fbx";
	std::string fullPath = path + fbxfile;

	// �t���O
	uint32_t flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs;

	// �V�[���̓ǂݍ���
	model->scene = model->importer.ReadFile(fullPath, flags);

	if (model->scene == nullptr)
	{
		assert(0 && "���f���̓ǂݍ��݂����s���܂���");
	}

	// �}�e���A���̉��
	NAssimpLoader::GetInstance()->ParseMaterial(model.get(), model->scene);
	NAssimpLoader::GetInstance()->ParseNodeRecursive(model.get(), nullptr, model->scene->mRootNode);

	// �o�b�t�@�[����
	model->mesh.vertexBuff.Init(model->mesh.vertices);
	model->mesh.indexBuff.Init(model->mesh.indices);

	// map�Ɋi�[
	sModelMap.insert(std::make_pair(modelHandle, std::move(model)));

	return sModelMap[modelHandle].get();
}
