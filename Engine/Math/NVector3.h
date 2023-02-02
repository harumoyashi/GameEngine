#pragma once

const float PI = 3.141592654f;

class NVector3
{
public:
	float x;
	float y;
	float z;

public:
	NVector3();								//�[���x�N�g���Ƃ���
	NVector3(float x, float y, float z);	//x����,y����,z�������w�肵�Ă̐���

	//�����o�ϐ�
	float Length() const;						//�m����(����)�����߂�
	NVector3 Normalize() const;						//���K��������
	float Dot(const NVector3& v) const;			//���ς����߂�
	NVector3 Cross(const NVector3& v) const;	//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	NVector3 operator+() const;
	NVector3 operator-() const;
	NVector3 operator*(const NVector3& v);
	NVector3 operator/(const NVector3& v);

	//������Z�q�I�[�o�[���[�h
	NVector3& operator+=(const NVector3& v);
	NVector3& operator-=(const NVector3& v);
	NVector3& operator*=(const NVector3& v);
	NVector3& operator/=(const NVector3& v);
	NVector3& operator*=(float s);
	NVector3& operator/=(float s);

	NVector3& operator=(float num);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ����̃p�^�[���ɑΉ�(�����̏���)���邽�߁A�ȉ��̂悤�ɏ������Ă���
const NVector3 operator+(const NVector3& v1, const NVector3& v2);
const NVector3 operator-(const NVector3& v1, const NVector3& v2);
const NVector3 operator*(const NVector3& v, float s);
const NVector3 operator*(float s, const NVector3& v);
const NVector3 operator/(const NVector3& v, float s);