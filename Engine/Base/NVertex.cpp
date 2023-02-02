#include "NVertex.h"
#include <cassert>

bool NVertexPNU::operator==(const NVertexPNU& a) const
{
	if (pos.x != a.pos.x|| pos.y != a.pos.y|| pos.z != a.pos.z)
	{
		return false;
	}
	else if (normal.x != a.normal.x || normal.y != a.normal.y || normal.z != a.normal.z)
	{
		return false;
	}
	else if (uv.x != a.uv.x || uv.y != a.uv.y)
	{
		return false;
	}

	return true;
}

void NVertexPNU::CalcNormalVec(std::vector<NVertexPNU> vertices, std::vector<unsigned int> indices)
{
	assert(indices.size() % 3 == 0);
	for (int i = 0; i < indices.size() / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�̃C���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɂ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		//�O�p�`���\�����钸�_���W���x�N�g���ɑ��
		NVector3 p0 = vertices[index0].pos;
		NVector3 p1 = vertices[index1].pos;
		NVector3 p2 = vertices[index2].pos;

		//p0��p1�x�N�g���Ap0��p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		NVector3 v1 = p1 - p0;
		NVector3 v2 = p2 - p0;

		//�O�ς͗������琂���ȃx�N�g��
		NVector3 normal = v1.Cross(v2);

		//���K��
		normal = normal.Normalize();

		//���߂��@���𒸓_�f�[�^�ɑ��
		vertices[index0].normal = normal;
		vertices[index1].normal = normal;
		vertices[index2].normal = normal;
	}
}
