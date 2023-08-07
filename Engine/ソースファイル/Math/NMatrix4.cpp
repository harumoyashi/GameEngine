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
		//�ő�̐�Βl��T������(�Ƃ肠�����Ώې������ő�Ɖ��肵�Ă���)
		float max = abs(mat[n][n]);
		size_t maxIndex = n;

		for (size_t i = n + 1; i < 4; i++) {
			if (abs(mat[i][n]) > max) {
				max = abs(mat[i][n]);
				maxIndex = i;
			}
		}

		//�ő�̐�Βl��0��������t�s��͋��߂��Ȃ�
		if (abs(mat[maxIndex][n]) <= 0.000001f) {
			return result; //�Ƃ肠�����P�ʍs��Ԃ����Ⴄ
		}

		//����ւ�
		if (n != maxIndex) {
			for (size_t i = 0; i < 8; i++) {
				float f = mat[maxIndex][i];
				mat[maxIndex][i] = mat[n][i];
				mat[n][i] = f;
			}
		}

		//�|������1�ɂȂ�l���Z�o
		float mul = 1 / mat[n][n];

		//�|����
		for (size_t i = 0; i < 8; i++) {
			mat[n][i] *= mul;
		}

		//���S��0�ɂ���
		for (size_t i = 0; i < 4; i++) {
			if (n == i) {
				continue;
			}

			float mul = -mat[i][n];

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

NMatrix4 NMatrix4::operator+(const NMatrix4& m) const
{
	NMatrix4 result(*this);
	result += m;
	return result;
}

NMatrix4& NMatrix4::operator+=(const NMatrix4& m)
{
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] += m.m[i][j];
		}
	}
	return *this;
}

NMatrix4 NMatrix4::operator-(const NMatrix4& m) const
{
	NMatrix4 result(*this);
	result -= m;
	return result;
}

NMatrix4& NMatrix4::operator-=(const NMatrix4& m)
{
	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			this->m[i][j] -= m.m[i][j];
		}
	}
	return *this;
}

NMatrix4 NMatrix4::operator*(const NMatrix4& m) const
{
	NMatrix4 result(*this);
	result *= m;
	return result;
}

NMatrix4& NMatrix4::operator*=(const NMatrix4& m)
{
	NMatrix4 result = *this;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			float f = 0;
			for (size_t k = 0; k < 4; k++)
			{
				f += result.m[i][k] * m.m[k][j];
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

NMatrix4 NMatrix4::Translation(const NVec3& t)
{
	NMatrix4 result = Identity();
	result.m[3][0] = t.x;
	result.m[3][1] = t.y;
	result.m[3][2] = t.z;
	return result;
}

NVec3 NMatrix4::Transform(const NVec3& v, const NMatrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	NVec3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0] * 0.0f) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1] * 0.0f) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] * 0.0f) / w
	};

	return result;
}

NVec3 NMatrix4::WTransform(const NVec3& v, const NMatrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	NVec3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

NVec4 NMatrix4::Vec4MulMat(const NVec4& v, const NMatrix4& m) const
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	NVec4 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2]) / w,
		(v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3]) / w
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

const NMatrix4 operator*(const NMatrix4& m1, const NMatrix4& m2)
{
	NMatrix4 result = m1;

	return result *= m2;
}

NVec3 operator*(const NVec3& v, const NMatrix4& m)
{
	NVec3 result = v;
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
	return result;
}

NVec4 operator*(const NVec4& v, const NMatrix4& m)
{
	NVec4 result = v;
	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2];
	result.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3];
	return result;
}

NVec3& operator*=(NVec3& v, const NMatrix4& m)
{
	NVec3 result = v * m;
	v = result;
	return v;
}
