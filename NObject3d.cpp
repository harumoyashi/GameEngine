#include "NObject3d.h"
#include <cassert>

void NObject3d::LoadFile()
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//.obj�t�@�C�����J��
	file.open("Resource/triangle/triangle.obj");
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3>positions;	//���_���W
	vector<XMFLOAT3>normals;	//�@���x�N�g��
	vector<XMFLOAT3>texcoords;	//�e�N�X�`��UV
	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');
	}
	//�t�@�C������
	file.close();
}
