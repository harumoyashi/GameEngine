#include "NVec4.h"
#include <cmath>

NVec4::NVec4() :x(0), y(0), z(0), w(0)
{
}

NVec4::NVec4(const float x, const float y, const float z, const float w) :x(x), y(y), z(z), w(w)
{
}

float NVec4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

NVec4 NVec4::Normalize() const
{
	float len = Length();
	if (len != 0)
	{
		return *this / len;
	}
	return *this;
}

float NVec4::Dot(const NVec4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

NVec4 NVec4::operator+() const
{
	return *this;
}

NVec4 NVec4::operator-() const
{
	return NVec4(-x, -y, -z,-w);
}

NVec4 NVec4::operator*(const NVec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

NVec4 NVec4::operator/(const NVec4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

NVec4& NVec4::operator+=(const NVec4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

NVec4& NVec4::operator-=(const NVec4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

NVec4& NVec4::operator*=(const NVec4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

NVec4& NVec4::operator/=(const NVec4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

NVec4& NVec4::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}

NVec4& NVec4::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;

	return *this;
}

NVec4& NVec4::operator=(const float num)
{
	x = num;
	y = num;
	z = num;
	w = num;

	return *this;
}

const NVec4 operator+(const NVec4& v1, const NVec4& v2)
{
	NVec4 temp(v1);
	return temp += v2;
}

const NVec4 operator-(const NVec4& v1, const NVec4& v2)
{
	NVec4 temp(v1);
	return temp -= v2;
}

const NVec4 operator*(const NVec4& v, const float s)
{
	NVec4 temp(v);
	return temp *= s;
}

const NVec4 operator*(const float s, const NVec4& v)
{
	return v * s;
}

const NVec4 operator/(const NVec4& v, const float s)
{
	NVec4 temp(v);
	return temp /= s;
}