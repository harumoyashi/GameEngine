#include "NMathUtil.h"
#include <math.h>
#include <random>

std::random_device seed;
std::mt19937 engine(seed());

NMatrix4 MathUtil::MatView(const NVector3& eye, const NVector3& target, const NVector3& up, const NVector3& rot)
{
	NMatrix4 mat;

	NVector3 direction = target - eye;
	direction = direction.Normalize();

	//外積使って向いてる方向に対しての右ベクトル求める
	NVector3 rightVec = up.Cross(direction);
	rightVec = rightVec.Normalize();
	//右ベクトルを使って上ベクトルも求める
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

	//NMatrix4 matRot;		//回転行列
	//NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rot.z));
	//NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rot.x));
	//NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rot.y));
	//matRot *= matZ;	//Z軸周りに回転してから
	//matRot *= matX;	//X軸周りに回転して
	//matRot *= matY;	//Y軸周りに回転

	////回転させてから返す
	//mat *= matRot;

	return mat;
}

NMatrix4 MathUtil::ParallelProjection(const float WIN_WIDTH, const float WIN_HEIGHT)
{
	return
	{
		2 / WIN_WIDTH,0,0,0,
		0,-2 / WIN_HEIGHT,0,0,
		0,0,1,0,
		-1,1,0,1
	};
}

NMatrix4 MathUtil::PerspectiveProjection(const float fov, const float aspect, const float nearZ, const float farZ)
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

float MathUtil::Degree2Radian(const float degree)
{
	return degree * PI / 180;
}

float MathUtil::Radian2Degree(const float radian)
{
	return radian * 180 / PI;
}

uint32_t MathUtil::Sign(const uint32_t value)
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

uint32_t MathUtil::Random(const uint32_t min, const uint32_t max)
{
	std::uniform_real_distribution<> dist1(min, max);

	return (uint32_t)dist1(engine);
}
