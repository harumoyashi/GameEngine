#include "NModelManager.h"
#include "NMtllib.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;

std::map<ModelHandle, uint32_t> NModelManager::sModelMap{};

std::vector<Model> NModelManager::modelDatas_{};
uint32_t NModelManager::indexModelData_ = 0u;

void NModelManager::AllLoad()
{
	LoadModel("sphere", "sphere");
	LoadModel("Cube", "cube");
	LoadModel("plane", "plane");
	LoadModel("busterSword", "busterSword");
	LoadModel("boss_model", "boss");
	LoadModel("cat", "cat");
}

Model NModelManager::GetModel(const std::string& modelHandle)
{
	uint32_t modelNum = sModelMap[modelHandle];
	return modelDatas_[modelNum];
}

uint32_t NModelManager::LoadModel(const std::string& modelname, const std::string& modelHandle)
{
	uint32_t handle = indexModelData_;

	// �ǂݍ��ݍς݃��f���f�[�^������
	auto it = std::find_if(modelDatas_.begin(), modelDatas_.end(), [&](const auto& modelData_) {
		return modelData_.name == modelname;
		});
	if (it != modelDatas_.end()) {
		// �ǂݍ��ݍς݃��f���f�[�^�̗v�f�ԍ����擾
		handle = static_cast<uint32_t>(std::distance(modelDatas_.begin(), it));
		return handle;
	}

	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	const string filename = modelname + ".obj";	//"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/";	//"Resources/triangle_mat/
	file.open(directoryPath + filename);	//"Resources/triangle_mat/triangle_mat.obj"
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	vector<NVector3>positions;	//���_���W
	vector<NVector3>normals;	//�@���x�N�g��
	vector<NVector2>texcoords;	//�e�N�X�`��UV
	modelDatas_.emplace_back();
	// �������ރ��f���f�[�^�̎Q��
	Model& modelData = modelDatas_.at(handle);
	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			modelData.material = NMtllib::Load(directoryPath, filename);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			NVector3 position{};
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
			NVector2 texcoord{};
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
			NVector3 normal{};
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
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);	//�X���b�V�����΂�
				index_stream >> indexNormal;

				//���_�f�[�^�ɒǉ�
				NVertexPNU vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelData.vertices.emplace_back(vertex);

				//�C���f�b�N�X�f�[�^�̒ǉ�
				modelData.indices.emplace_back((unsigned short)modelData.indices.size());
			}
		}
	}
	//�t�@�C������
	file.close();

	//�o�b�t�@�̐���
	modelData.vertexBuff.Init(modelData.vertices);
	modelData.indexBuff.Init(modelData.indices);
	//���f������o�^����
	modelData.name = modelname;

	//�����I�Ƀn���h�������ɐi�߂�
	indexModelData_++;

	//���f���}�b�v�ɓo�^
	sModelMap.insert(std::make_pair(modelHandle, handle));
	return handle;
}