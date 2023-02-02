#pragma once
#include <math.h>
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
	//�ŏ��A�ő�l�ݒ�
	float Clamp(float value, float max, float min);
}