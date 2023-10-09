#include "NMatrix4.h"
#include "NMathUtil.h"
#include <cmath>

NMatrix4 NMatrix4::operator-() const
{
	NMatrix4 result;
	float mat[4][8] = { 0 };

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			mat[i][j] = m[i][j];
		}
	}

	mat[0][4] = 1;
	mat[1][5] = 1;
	mat[2][6] = 1;
	mat[3][7] = 1;

	for (size_t n = 0; n < 4; n++) {
		//最大の絶対値を探索する(とりあえず対象成分を最大と仮定しておく)
		float max = abs(mat[n][n]);
		size_t maxIndex = n;

		for (size_t i = n + 1; i < 4; i++) {
			if (abs(mat[i][n]) > max) {
				max = abs(mat[i][n]);
				maxIndex = i;
			}
		}

		//最大の絶対値が0だったら逆行列は求められない
		if (abs(mat[maxIndex][n]) <= 0.000001f) {
			return result; //とりあえず単位行列返しちゃう
		}

		//入れ替え
		if (n != maxIndex) {
			for (size_t i = 0; i < 8; i++) {
				float f = mat[maxIndex][i];
				mat[maxIndex][i] = mat[n][i];
				mat[n][i] = f;
			}
		}

		//掛けたら1になる値を算出
		float mul = 1 / mat[n][n];

		//掛ける
		for (size_t i = 0; i < 8; i++) {
			mat[n][i] *= mul;
		}

		//他全部0にする
		for (size_t i = 0; i < 4; i++) {
			if (n == i) {
				continue;
			}

			mul = -mat[i][n];

			for (size_t j = 0; j < 8; j++) {
				mat[i][j] += mat[n][j] * mul;
			}
		}
	}

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			result.m[i][j] = mat[i][j + 4];
		}
	}

	return result;
}

NMatrix4 NMatrix4::operator+(const NMatrix4& mat) const
{
	NMatrix4 result(*this);
	result += mat;
	return result;
}

NMatrix4& NMatrix4::operator+=(const NMatrix4& mat)
{
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] += mat.m[i][j];
		}
	}
	return *this;
}

NMatrix4 NMatrix4::operator-(const NMatrix4& mat) const
{
	NMatrix4 result(*this);
	result -= mat;
	return result;
}

NMatrix4& NMatrix4::operator-=(const NMatrix4& mat)
{
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] -= mat.m[i][j];
		}
	}
	return *this;
}

NMatrix4 NMatrix4::operator*(const NMatrix4& mat) const
{
	NMatrix4 result(*this);
	result *= mat;
	return result;
}

NMatrix4& NMatrix4::operator*=(const NMatrix4& mat)
{
	NMatrix4 result = *this;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			float f = 0;
			for (size_t k = 0; k < 4; k++)
			{
				f += result.m[i][k] * mat.m[k][j];
			}
			this->m[i][j] = f;
		}
	}
	return *this;
}

NMatrix4 NMatrix4::Zero()
{
	return {
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};
}

NMatrix4 NMatrix4::Identity()
{
	NMatrix4 result{};
	result.m[0][0] = 1;
	result.m[1][1] = 1;
	result.m[2][2] = 1;
	result.m[3][3] = 1;
	return result;
}

NMatrix4 NMatrix4::Scale(const NVec3& s)
{
	NMatrix4 result = Identity();
	result.m[0][0] = s.x;
	result.m[1][1] = s.y;
	result.m[2][2] = s.z;
	return result;
}

NMatrix4 NMatrix4::RotateX(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	NMatrix4 result = Identity();
	result.m[1][1] = cos;
	result.m[1][2] = sin;
	result.m[2][1] = -sin;
	result.m[2][2] = cos;
	return result;
}

NMatrix4 NMatrix4::RotateY(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	NMatrix4 result = Identity();
	result.m[0][0] = cos;
	result.m[0][2] = -sin;
	result.m[2][0] = sin;
	result.m[2][2] = cos;
	return result;
}

NMatrix4 NMatrix4::RotateZ(const float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	NMatrix4 result = Identity();
	result.m[0][0] = cos;
	result.m[0][1] = sin;
	result.m[1][0] = -sin;
	result.m[1][1] = cos;
	return result;
}

NMatrix4 NMatrix4::RotateQuaternion(const NQuaternion& q)
{
	NMatrix4 result = NMatrix4::Identity();

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float ww = q.w * q.w;
	float xy = q.x * q.y * 2.0f;
	float xz = q.x * q.z * 2.0f;
	float yz = q.y * q.z * 2.0f;
	float wx = q.w * q.x * 2.0f;
	float wy = q.w * q.y * 2.0f;
	float wz = q.w * q.z * 2.0f;

	result =
	{
		ww + xx - yy - zz, xy + wz          , xz - wy          ,0.0f,
		xy - wz          , ww - xx + yy - zz, yz + wx          ,0.0f,
		xz + wy          , yz - wx          , ww - xx - yy + zz,0.0f,
		0.0f             ,0.0f              ,0.0f              ,1.0f
	};

	return result;
}

NMatrix4 NMatrix4::Translation(const NVec3& t)
{
	NMatrix4 result = Identity();
	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;
	return result;
}

NVec3 NMatrix4::Transform(const NVec3& v, const NMatrix4& mat)
{
	float w = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3];

	NVec3 result
	{
		(v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0] * 0.0f) / w,
		(v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1] * 0.0f) / w,
		(v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2] * 0.0f) / w
	};

	return result;
}

NVec3 NMatrix4::WTransform(const NVec3& v, const NMatrix4& mat)
{
	float w = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3];

	NVec3 result
	{
		(v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + mat.m[3][0]) / w,
		(v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + mat.m[3][1]) / w,
		(v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + mat.m[3][2]) / w
	};

	return result;
}

NVec4 NMatrix4::Vec4MulMat(const NVec4& v, const NMatrix4& mat) const
{
	float w = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3];

	NVec4 result
	{
		(v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + v.w * mat.m[3][0]) / w,
		(v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + v.w * mat.m[3][1]) / w,
		(v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + v.w * mat.m[3][2]) / w,
		(v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + mat.m[3][3]) / w
	};

	return result;
}

NVec3 NMatrix4::ToEuler() const
{
	NVec3 euler;

	euler.x = asinf(m[1][2]);
	if (cosf(euler.x) != 0.0f)
	{
		euler.y = atanf(m[0][2] / m[2][2]);
		euler.z = atanf(m[1][0] / m[1][1]);
	}
	else
	{
		euler.y = atanf(m[2][0] / m[0][0]);
		euler.z = 0.0f;
	}

	return euler;
}

NMatrix4 NMatrix4::Transpose()
{
	NMatrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m[j][i];
		}
	}
	return result;
}

NVec3 NMatrix4::GetWorldPos() const
{
	NVec3 pos = {};

	float x = m[3][0];
	float y = m[3][1];
	float z = m[3][2];

	pos = { x,y,z };

	return pos;
}

NVec3 NMatrix4::GetScale() const
{
	NVec3 scale = {};

	float x = sqrtf(powf(m[0][0], 2) + powf(m[0][1], 2) + powf(m[0][2], 2));
	float y = sqrtf(powf(m[1][0], 2) + powf(m[1][1], 2) + powf(m[1][2], 2));
	float z = sqrtf(powf(m[2][0], 2) + powf(m[2][1], 2) + powf(m[2][2], 2));

	scale = { x,y,z };

	return scale;
}

const NMatrix4 operator*(const NMatrix4& mat1, const NMatrix4& mat2)
{
	NMatrix4 result = mat1;

	return result *= mat2;
}

NVec3 operator*(const NVec3& v, const NMatrix4& mat)
{
	NVec3 result = v;
	result.x = v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0];
	result.y = v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1];
	result.z = v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2];
	return result;
}

NVec4 operator*(const NVec4& v, const NMatrix4& mat)
{
	NVec4 result = v;
	result.x = v.x * mat.m[0][0] + v.y * mat.m[1][0] + v.z * mat.m[2][0] + v.w * mat.m[3][0];
	result.y = v.x * mat.m[0][1] + v.y * mat.m[1][1] + v.z * mat.m[2][1] + v.w * mat.m[3][1];
	result.z = v.x * mat.m[0][2] + v.y * mat.m[1][2] + v.z * mat.m[2][2] + v.w * mat.m[3][2];
	result.w = v.x * mat.m[0][3] + v.y * mat.m[1][3] + v.z * mat.m[2][3] + v.w * mat.m[3][3];
	return result;
}

NVec3& operator*=(NVec3& v, const NMatrix4& mat)
{
	NVec3 result = v * mat;
	v = result;
	return v;
}
