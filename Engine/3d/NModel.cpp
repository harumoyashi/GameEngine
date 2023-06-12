#include "NModel.h"
#include "NMtllib.h"

#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void NModel::Create(const string modelname)
{
	LoadObjFile(modelname);
	vertexBuff_.Init(vertices_);
	indexBuff_.Init(indices_);
}

void NModel::LoadObjFile(const string modelname)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	const string filename = modelname + ".obj";	//"triangle_mat.obj"
	const string dir_ectoryPath = "Resources/" + modelname + "/";	//"Resources/triangle_mat/
	file.open(dir_ectoryPath + filename);	//"Resources/triangle_mat/triangle_mat.obj"
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	vector<NVector3>positions;	//���_���W
	vector<NVector3>normals;	//�@���x�N�g��
	vector<NVector2>texcoords;	//�e�N�X�`��UV
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
			material_ = NMtllib::Load(dir_ectoryPath,filename);
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
				vertices_.emplace_back(vertex);

				//�C���f�b�N�X�f�[�^�̒ǉ�
				indices_.emplace_back((unsigned short)indices_.size());
			}
		}
	}
	//���f������o�^����
	name_ = modelname;
	//�t�@�C������
	file.close();
}