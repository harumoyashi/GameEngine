#include "NMathUtil.h"
#include <math.h>
#include <random>

std::random_device seed;
std::mt19937 engine(seed());

NMatrix4 MathUtil::MatView(NVector3 eye, NVector3 target, NVector3 up)
{
	NMatrix4 mat;

	NVector3 dir_ection = target - eye;
	dir_ection = dir_ection.Normalize();

	//外積使って向いてる方向に対しての右ベクトル求める
	NVector3 rightVec = up.Cross(dir_ection);
	rightVec = rightVec.Normalize();
	//右ベクトルを使って上ベクトルも求める
	NVector3 upVec = dir_ection.Cross(rightVec);
	upVec = upVec.Normalize();

	mat.m[0][0] = rightVec.x;
	mat.m[0][1] = rightVec.y;
	mat.m[0][2] = rightVec.z;
	mat.m[1][0] = upVec.x;
	mat.m[1][1] = upVec.y;
	mat.m[1][2] = upVec.z;
	mat.m[2][0] = dir_ection.x;
	mat.m[2][1] = dir_ection.y;
	mat.m[2][2] = dir_ection.z;
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
	// 単位行列で初期化
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

uint32_t MathUtil::Sign(uint32_t value)
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

uint32_t MathUtil::Random(uint32_t min, uint32_t max)
{
	std::uniform_real_distribution<> dist1(min, max);

	return (uint32_t)dist1(engine);
}
