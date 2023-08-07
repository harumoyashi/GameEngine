#pragma once
struct NVec4
{
	float x;
	float y;
	float z;
	float w;

	NVec4();								//�[���x�N�g���Ƃ���
	NVec4(const float x, const float y, const float z, const float w);	//x����,y����,z����,w�������w�肵�Ă̐���

	//�����o�ϐ�
	float Length() const;						//�m����(����)�����߂�
	NVec4 Normalize() const;					//���K��������
	float Dot(const NVec4& v) const;			//���ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	NVec4 operator+() const;
	NVec4 operator-() const;
	NVec4 operator*(const NVec4& v);
	NVec4 operator/(const NVec4& v);

	//������Z�q�I�[�o�[���[�h
	NVec4& operator+=(const NVec4& v);
	NVec4& operator-=(const NVec4& v);
	NVec4& operator*=(const NVec4& v);
	NVec4& operator/=(const NVec4& v);
	NVec4& operator*=(const float s);
	NVec4& operator/=(const float s);

	NVec4& operator=(const float num);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVec4 operator+(const NVec4& v1, const NVec4& v2);
const NVec4 operator-(const NVec4& v1, const NVec4& v2);
const NVec4 operator*(const NVec4& v, const float s);
const NVec4 operator*(const float s, const NVec4& v);
const NVec4 operator/(const NVec4& v, const float s);