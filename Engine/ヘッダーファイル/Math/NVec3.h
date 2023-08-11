#pragma once

struct NVec3
{
	float x;
	float y;
	float z;

	NVec3();								//�[���x�N�g���Ƃ���
	NVec3(const float x, const float y, const float z);	//x����,y����,z�������w�肵�Ă̐���

	//����y����p
	const static NVec3 zero;
	const static NVec3 one;
	const static NVec3 up;
	const static NVec3 front;
	const static NVec3 right;

	//�����o�֐�
	float Length() const;						//�m����(����)�����߂�
	NVec3 Normalize() const;					//���K��������
	float Dot(const NVec3& v) const;			//���ς����߂�
	NVec3 Cross(const NVec3& v) const;	//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	NVec3 operator+() const;
	NVec3 operator-() const;
	NVec3 operator*(const NVec3& v);
	NVec3 operator/(const NVec3& v);

	//������Z�q�I�[�o�[���[�h
	NVec3& operator+=(const NVec3& v);
	NVec3& operator-=(const NVec3& v);
	NVec3& operator*=(const NVec3& v);
	NVec3& operator/=(const NVec3& v);
	NVec3& operator*=(const float s);
	NVec3& operator/=(const float s);

	NVec3& operator=(float num);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVec3 operator+(const NVec3& v1, const NVec3& v2);
const NVec3 operator-(const NVec3& v1, const NVec3& v2);
const NVec3 operator*(const NVec3& v, const float s);
const NVec3 operator*(const float s, const NVec3& v);
const NVec3 operator/(const NVec3& v, const float s);