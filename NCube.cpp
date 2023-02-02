#include "NCube.h"

////�ÓI�����o�ϐ��̎���
//std::vector<Vertex> NCube::vertices;
//std::vector<unsigned short>NCube::indices;


void NCube::Init(int modelNum)
{
	//���_
	if (modelNum == CUBE)
	{
		SetVertCube();
		SetIndexCube();	//�����ŃC���f�b�N�X�̐ݒ肾���s��
	}
	else if (modelNum == CRYSTAL)
	{
		SetVertCrystal();
		SetIndexCrystal();	//�����ŃC���f�b�N�X�̐ݒ肾���s��
	}
	vertexBuff.Init(vertices);
	indexBuff.Init(indices);
}

void NCube::SetVertCube()
{
	// ���_�f�[�^�ݒ�
	vertices = {
		//	x		y		z	 �@��	u	v
		//�O
		{{ -1.0f, -1.0f, -1.0f }, {}, {0.0f,1.0f}},	// ����
		{{ -1.0f,  1.0f, -1.0f }, {}, {0.0f,0.0f}},	// ����
		{{  1.0f, -1.0f, -1.0f }, {}, {1.0f,1.0f}},	// �E��
		{{  1.0f,  1.0f, -1.0f }, {}, {1.0f,0.0f}},	// �E��

		//��
		{{ -1.0f, -1.0f, 1.0f }, {}, {0.0f,1.0f}},	// ����
		{{ -1.0f,  1.0f, 1.0f }, {}, {0.0f,0.0f}},	// ����
		{{  1.0f, -1.0f, 1.0f }, {}, {1.0f,1.0f}},	// �E��
		{{  1.0f,  1.0f, 1.0f }, {}, {1.0f,0.0f}},	// �E��

		 // ��
		{{-1.0f,-1.0f,-1.0f }, {}, {0.0f, 1.0f}},    // ����
		{{-1.0f,-1.0f, 1.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-1.0f, 1.0f,-1.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{-1.0f, 1.0f, 1.0f }, {}, {1.0f, 0.0f}},    // �E��

		// �E
		{{ 1.0f,-1.0f,-1.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 1.0f,-1.0f, 1.0f }, {}, {0.0f, 0.0f}},    // ����
		{{ 1.0f, 1.0f,-1.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 1.0f, 1.0f, 1.0f }, {}, {1.0f, 0.0f}},    // �E��

		// ��
		{{-1.0f,-1.0f,-1.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 1.0f,-1.0f,-1.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-1.0f,-1.0f, 1.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 1.0f,-1.0f, 1.0f }, {}, {1.0f, 0.0f}},    // �E��

		// ��
		{{-1.0f, 1.0f,-1.0f }, {}, {0.0f, 1.0f}},    // ����
		{{ 1.0f, 1.0f,-1.0f }, {}, {0.0f, 0.0f}},    // ����
		{{-1.0f, 1.0f, 1.0f }, {}, {1.0f, 1.0f}},    // �E��
		{{ 1.0f, 1.0f, 1.0f }, {}, {1.0f, 0.0f}},    // �E��
	};

	////���_�o�b�t�@�̃T�C�Y����
	//singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	//sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
}

void NCube::SetVertCrystal()
{
	// ���_�f�[�^�ݒ�
	vertices = {
		//	x		y		z	 �@��	u	v
			//���O
		{ { -0.0f, +2.0f, -0.0f }, {}, { 0.0f,0.0f }},	// ��
		{ {  0.0f, -0.0f, -1.0f }, {}, {0.0f,0.0f} },	// �E��
		{ { -1.0f,  0.0f, -0.0f }, {}, {0.0f,0.0f} },	// ����
		{ { -0.0f, -2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��

		//�E�O
		{ { -0.0f, +2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��
		{ { +1.0f,  0.0f, -0.0f }, {}, {0.0f,0.0f} },	// �E��
		{ {  0.0f, -0.0f, -1.0f }, {}, {0.0f,0.0f} },	// ����
		{ { -0.0f, -2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��

		//����
		{ { -0.0f, +2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��
		{ { -1.0f,  0.0f, -0.0f }, {}, {0.0f,0.0f} },	// �E��
		{ { -0.0f,  0.0f, +1.0f }, {}, {0.0f,0.0f} },	// ����
		{ { -0.0f, -2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��

		//�E��
		{ { -0.0f, +2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��
		{ { -0.0f,  0.0f, +1.0f }, {}, {0.0f,0.0f} },	// �E��
		{ { +1.0f,  0.0f, -0.0f }, {}, {0.0f,0.0f} },	// ����
		{ { -0.0f, -2.0f, -0.0f }, {}, {0.0f,0.0f} },	// ��
	};

	////���_�o�b�t�@�̃T�C�Y����
	//singleSizeVB = static_cast<UINT>(sizeof(vertices[0]));
	//sizeVB = static_cast<UINT>(sizeof(Vertex) * vertices.size());
}

void NCube::SetIndexCube()
{
	//�C���f�b�N�X�f�[�^
	indices =
	{
		//�O
		0,1,2,	//�O�p�`1��
		2,1,3,	//�O�p�`2��
		//��
		5,4,6,	//�O�p�`3��
		5,6,7,	//�O�p�`4��
		//��
		8,9,10,	//�O�p�`5��
		10,9,11,//�O�p�`6��
		//�E
		13,12,14,	//�O�p�`7��
		13,14,15,	//�O�p�`8��
		//��
		16,17,18,	//�O�p�`9��
		18,17,19,	//�O�p�`10��
		//��
		21,20,22,	//�O�p�`11��
		21,22,23,	//�O�p�`12��
	};

	////���_�o�b�t�@�̃T�C�Y����
	//sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	//numIB = static_cast<UINT>(sizeof(unsigned short) * indices.size() / sizeof(indices[0]));
}

void NCube::SetIndexCrystal()
{
	//�C���f�b�N�X�f�[�^
	indices =
	{
		//���O
		0, 1, 2,	//�O�p�`1��
		2, 1, 3,	//�O�p�`2��
		//�E�O
		4, 5, 6,	//�O�p�`3��
		6, 5, 7,	//�O�p�`4��
		//����
		8, 9, 10,	//�O�p�`5��
		10, 9, 11,	//�O�p�`6��
		//�E��
		12, 13, 14,	//�O�p�`7��
		14, 13, 15,	//�O�p�`8��
	};

	////���_�o�b�t�@�̃T�C�Y����
	//sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	//numIB = static_cast<UINT>(sizeof(unsigned short) * indices.size() / sizeof(indices[0]));
}