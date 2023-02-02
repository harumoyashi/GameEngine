#pragma once
#include "NVector3.h"

class NMatrix4
{
public:
	float m[4][4];

	//�P�ʍs��Ő���
	NMatrix4() :
		m{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 }{}
	NMatrix4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33):
		m {
			_00, _01, _02, _03,
			_10, _11, _12, _13,
			_20, _21, _22, _23,
			_30, _31, _32, _33}{}

	//�t�s��
	NMatrix4 operator-() const;

	//������Z�q�I�[�o�[���[�h
	//���Z
	NMatrix4 operator+(const NMatrix4& m) const;
	//���Z
	NMatrix4& operator+=(const NMatrix4& m);
	//���Z
	NMatrix4 operator-(const NMatrix4& m) const;
	//���Z
	NMatrix4& operator-=(const NMatrix4& m);
	//��Z
	NMatrix4 operator*(const NMatrix4& m) const;
	//��Z
	NMatrix4& operator*=(const NMatrix4& m);

	//�S�Ă̗v�f��0�̍s��
	NMatrix4 Zero();
	//�P�ʍs��
	NMatrix4 Identity();
	//�g�k�s��̐ݒ�
	NMatrix4 Scale(const NVector3& s);
	//��]�s��̐ݒ�
	NMatrix4 RotateX(float angle);
	NMatrix4 RotateY(float angle);
	NMatrix4 RotateZ(float angle);
	//���s�ړ��s��̍쐬
	NMatrix4 Translation(const NVector3& t);
	//���W�ϊ��i�x�N�g���ƍs��̊|���Z�j
	NVector3 Transform(const NVector3& v, const NMatrix4& m);
	//w���Z���g�������W�ϊ�
	NVector3 WTransform(const NVector3& v, const NMatrix4& m);

	//�I�C���[�p�ւ̕ϊ�
	//�Q�l:https://qiita.com/aa_debdeb/items/3d02e28fb9ebfa357eaf#%E5%9B%9E%E8%BB%A2%E8%A1%8C%E5%88%97%E3%81%8B%E3%82%89%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E8%A7%92
	NVector3 ToEuler() const;
};

NVector3 operator*(const NVector3 v, const NMatrix4 m);
NVector3& operator*=(NVector3& v, const NMatrix4 m);