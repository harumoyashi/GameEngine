#pragma once
struct NVector4
{
	float x;
	float y;
	float z;
	float w;

	NVector4();								//�[���x�N�g���Ƃ���
	NVector4(const float x, const float y, const float z, const float w);	//x����,y����,z����,w�������w�肵�Ă̐���

	//�����o�ϐ�
	float Length() const;						//�m����(����)�����߂�
	NVector4 Normalize() const;					//���K��������
	float Dot(const NVector4& v) const;			//���ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	NVector4 operator+() const;
	NVector4 operator-() const;
	NVector4 operator*(const NVector4& v);
	NVector4 operator/(const NVector4& v);

	//������Z�q�I�[�o�[���[�h
	NVector4& operator+=(const NVector4& v);
	NVector4& operator-=(const NVector4& v);
	NVector4& operator*=(const NVector4& v);
	NVector4& operator/=(const NVector4& v);
	NVector4& operator*=(const float s);
	NVector4& operator/=(const float s);

	NVector4& operator=(const float num);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVector4 operator+(const NVector4& v1, const NVector4& v2);
const NVector4 operator-(const NVector4& v1, const NVector4& v2);
const NVector4 operator*(const NVector4& v, const float s);
const NVector4 operator*(const float s, const NVector4& v);
const NVector4 operator/(const NVector4& v, const float s);