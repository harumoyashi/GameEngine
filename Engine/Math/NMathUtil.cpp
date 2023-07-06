#include "NMathUtil.h"
#include <math.h>
#include <random>

std::random_device seed;
std::mt19937 engine(seed());

NMatrix4 MathUtil::MatViewLockTo(const NVector3& eye, const NVector3& front, const NVector3& up)
{
	NMatrix4 mat;

	//外積使って向いてる方向に対しての右ベクトル求める
	NVector3 rightVec = up.Cross(front);
	rightVec = rightVec.Normalize();
	
	mat.m[0][0] = rightVec.x;
	mat.m[0][1] = rightVec.y;
	mat.m[0][2] = rightVec.z;
	mat.m[1][0] = up.x;
	mat.m[1][1] = up.y;
	mat.m[1][2] = up.z;
	mat.m[2][0] = front.x;
	mat.m[2][1] = front.y;
	mat.m[2][2] = front.z;
	mat.m[3][0] = eye.x;
	mat.m[3][1] = eye.y;
	mat.m[3][2] = eye.z;

	mat = -mat;
	return mat;
}

NMatrix4 MathUtil::MatViewLockAt(const NVector3& eye, const NVector3& target, const NVector3& up)
{
	NVector3 direction = target - eye;
	direction = direction.Normalize();

	return MatViewLockTo(eye, direction, up);
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

float MathUtil::Randomf(const float min, const float max)
{
	std::uniform_real_distribution<> dist1(min, max);

	return (float)dist1(engine);
}

NVector2 MathUtil::CircleMotion(NVector2 CenterPoint, float radius, float angle)
{
	NVector2 position;

	position.x = CenterPoint.x + radius * cosf(angle);
	position.y = CenterPoint.y + radius * sinf(angle);

	return position;
}
