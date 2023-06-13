#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"
#include "NQuaternion.h"
#include <stdint.h>

const float PI = 3.141592654f;
const float PI2 = PI * 2.0f;

namespace MathUtil
{
	//�r���[�ϊ��s�񐶐�
	NMatrix4 MatView(const NVector3& eye, const NVector3& target, const NVector3& up);
	//���s���e�ϊ��s�񐶐�
	NMatrix4 ParallelProjection(const float WIN_WIDTH, const float WIN_HEIGHT);
	//�������e�ϊ��s�񐶐�
	NMatrix4 PerspectiveProjection(const float fov, const float aspect, const  float nearZ, const float farZ);

	//�x���@�����W�A��
	float Degree2Radian(const float degree);
	//�x���@�����W�A��
	float Radian2Degree(const float radian);
	//�����Ԃ�
	uint32_t Sign(const uint32_t value);
	//�����_���Ȓl��Ԃ�
	uint32_t Random(const uint32_t min, const uint32_t max);
	//�ŏ��A�ő�l�ݒ�
	template <typename Type>
	Type Clamp(const Type value, const Type min, const Type max)
	{
		// �l���ő�l�������Ă�����ő�l��Ԃ�
		if (value >= max) return max;

		// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
		if (value <= min) return min;

		// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
		return static_cast<Type>(value);
	}
}