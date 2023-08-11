#include "NVec2.h"
#include <cmath>    // sqrt

NVec2::NVec2() :x(0), y(0) {}

NVec2::NVec2(const float x, const float y) : x(x), y(y) {}

float NVec2::Length() const
{
	return sqrtf(x * x + y * y);
}

NVec2& NVec2::Normalize()
{
	float len = Length();
	if (len != 0)
	{
		return *this /= len;
	}
	return *this;
}

float NVec2::Dot(const NVec2& v) const
{
	return x * v.x + y * v.y;
}

float NVec2::Cross(const NVec2& v) const
{
	return x * v.y - y * v.x;
}

NVec2 NVec2::operator+() const
{
	return *this;
}

NVec2 NVec2::operator-() const
{
	return NVec2(-x, -y);
}

NVec2& NVec2::operator+=(const NVec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

NVec2& NVec2::operator-=(const NVec2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

NVec2& NVec2::operator*=(const float s)
{
	x *= s;
	y *= s;
	return *this;
}

NVec2& NVec2::operator/=(const float s)
{
	x /= s;
	y /= s;
	return *this;
}

// NVec2 ƒNƒ‰ƒX‚É‘®‚³‚È‚¢ŠÖ”ŒR
// “ñ€‰‰Zq
const NVec2 operator+(const NVec2& v1, const NVec2& v2)
{
	NVec2 temp(v1);
	return temp += v2;
}

const NVec2 operator-(const NVec2& v1, const NVec2& v2)
{
	NVec2 temp(v1);
	return temp -= v2;
}

const NVec2 operator*(const NVec2& v, const float s)
{
	NVec2 temp(v);
	return temp *= s;
}

const NVec2 operator*(const float s, const NVec2& v)
{
	return v * s;
}

const NVec2 operator/(const NVec2& v, const float s)
{
	NVec2 temp(v);
	return temp /= s;
}