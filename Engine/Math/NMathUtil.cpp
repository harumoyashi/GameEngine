#include "NMathUtil.h"

NMatrix4 MathUtil::MatView(NVector3 eye, NVector3 target, NVector3 up)
{
	NMatrix4 mat;

	NVector3 direction = target - eye;
	direction = direction.Normalize();

	//�O�ώg���Č����Ă�����ɑ΂��ẲE�x�N�g�����߂�
	NVector3 rightVec = up.Cross(direction);
	rightVec = rightVec.Normalize();
	//�E�x�N�g�����g���ď�x�N�g�������߂�
	NVector3 upVec = direction.Cross(rightVec);
	upVec = upVec.Normalize();

	mat.m[0][0] = rightVec.x;
	mat.m[0][1] = rightVec.y;
	mat.m[0][2] = rightVec.z;
	mat.m[1][0] = upVec.x;
	mat.m[1][1] = upVec.y;
	mat.m[1][2] = upVec.z;
	mat.m[2][0] = direction.x;
	mat.m[2][1] = direction.y;
	mat.m[2][2] = direction.z;
	mat.m[3][0] = eye.x;
	mat.m[3][1] = eye.y;
	mat.m[3][2] = eye.z;

	mat = -mat;
	return mat;
}

NMatrix4 MathUtil::ParallelProjection(float WIN_WIDTH, float WIN_HEIGHT)
{
	return
	{
		2 / WIN_WIDTH,0,0,0,
		0,-2 / WIN_HEIGHT,0,0,
		0,0,1,0,
		-1,1,0,1
	};
}

NMatrix4 MathUtil::PerspectiveProjection(float fov, float aspect, float nearZ, float farZ)
{
	// �P�ʍs��ŏ�����
	NMatrix4 perspective = perspective.Zero();

	float scaleY = 1 / tanf(fov / 2);
	float scaleX = 1 / tanf(fov / 2) / aspect;
	float scaleZ = 1 / (farZ - nearZ) * farZ;
	float TransZ = -nearZ / (farZ - nearZ) * farZ;

	perspective.m[1][1] = scaleY;
	perspective.m[0][0] = scaleX;
	perspective.m[2][2] = scaleZ;
	perspective.m[3][2] = TransZ;
	perspective.m[2][3] = 1;

	return perspective;
}

float MathUtil::Degree2Radian(float degree)
{
	return degree * PI / 180;
}

float MathUtil::Radian2Degree(float radian)
{
	return radian * 180 / PI;
}

int MathUtil::Sign(int value)
{
	if (value > 0)
	{
		return 1;
	}
	else if (value < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

float MathUtil::Sign(float value)
{
	if (value > 0.0f)
	{
		return 1.0f;
	}
	else if (value < 0.0f)
	{
		return -1.0f;
	}
	else
	{
		return 0.0f;
	}
}

float MathUtil::Clamp(float value, float max, float min)
{
	// �l���ő�l�������Ă�����ő�l��Ԃ�
	if (value >= max) return max;

	// �l���ŏ��l��������Ă�����ŏ��l��Ԃ�
	if (value <= min) return min;

	// �ǂ���ɂ����Ă͂܂�Ȃ�������l�����̂܂ܕԂ�
	return value;
}
