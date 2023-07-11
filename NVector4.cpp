#include "NVector4.h"
#include <cmath>

NVector4::NVector4() :x(0), y(0), z(0), w(0)
{
}

NVector4::NVector4(const float x, const float y, const float z, const float w) :x(x), y(y), z(z), w(w)
{
}

float NVector4::Length() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

NVector4 NVector4::Normalize() const
{
	float len = Length();
	if (len != 0)
	{
		return *this / len;
	}
	return *this;
}

float NVector4::Dot(const NVector4& v) const
{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

NVector4 NVector4::operator+() const
{
	return *this;
}

NVector4 NVector4::operator-() const
{
	return NVector4(-x, -y, -z,-w);
}

NVector4 NVector4::operator*(const NVector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

NVector4 NVector4::operator/(const NVector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

NVector4& NVector4::operator+=(const NVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}

NVector4& NVector4::operator-=(const NVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}

NVector4& NVector4::operator*=(const NVector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}

NVector4& NVector4::operator/=(const NVector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}

NVector4& NVector4::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}

NVector4& NVector4::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;

	return *this;
}

NVector4& NVector4::operator=(const float num)
{
	x = num;
	y = num;
	z = num;
	w = num;

	return *this;
}

const NVector4 operator+(const NVector4& v1, const NVector4& v2)
{
	NVector4 temp(v1);
	return temp += v2;
}

const NVector4 operator-(const NVector4& v1, const NVector4& v2)
{
	NVector4 temp(v1);
	return temp -= v2;
}

const NVector4 operator*(const NVector4& v, const float s)
{
	NVector4 temp(v);
	return temp *= s;
}

const NVector4 operator*(const float s, const NVector4& v)
{
	return v * s;
}

const NVector4 operator/(const NVector4& v, const float s)
{
	NVector4 temp(v);
	return temp /= s;
}