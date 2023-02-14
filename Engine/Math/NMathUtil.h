#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"
#include "NQuaternion.h"

namespace MathUtil
{
	//�r���[�ϊ��s�񐶐�
	NMatrix4 MatView(NVector3 eye, NVector3 target, NVector3 up);
	//���s���e�ϊ��s�񐶐�
	NMatrix4 ParallelProjection(float WIN_WIDTH, float WIN_HEIGHT);
	//�������e�ϊ��s�񐶐�
	NMatrix4 PerspectiveProjection(float fov, float aspect, float nearZ, float farZ);

	//�x���@�����W�A��
	float Degree2Radian(float degree);
	//�x���@�����W�A��
	float Radian2Degree(float radian);
	//�����Ԃ�
	int Sign(int value);
	//�����Ԃ�
	float Sign(float value);
	//�����_���Ȓl��Ԃ�
	int Random(int min,int max);
	//�����_���Ȓl��Ԃ�
	float Random(float min,float max);
	//�ŏ��A�ő�l�ݒ�
	template <typename Type>
	Type Clamp(Type value, Type min, Type max)
	{
		// �l���ő�l�������Ă�����ő�l��Ԃ�
		if (value >= max) return max;

		// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
		if (value <= min) return min;

		// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
		return static_cast<Type>(value);
	}
}