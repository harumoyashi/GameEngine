#include "NVec3.h"
#include <cmath>

const NVec3 NVec3::zero(0,0,0);
const NVec3 NVec3::one(1.f,1.f,1.f);
const NVec3 NVec3::up(0,1.f,0);
const NVec3 NVec3::front(0,0,1.f);
const NVec3 NVec3::right(1.f,0,0);

NVec3::NVec3():x(0),y(0),z(0) {

}

NVec3::NVec3(const float x, const float y, const float z) :x(x), y(y), z(z) {

}

float NVec3::Length() const {
	return sqrtf(x * x + y * y + z * z);
}

NVec3 NVec3::Normalize() const {
	float len = Length();
	if (len != 0)
	{
		return *this / len;
	}
	return *this;
}

NVec3 NVec3::Cross(const NVec3& v) const
{
	NVec3 vec3;
	vec3.x = y * v.z - z * v.y;
	vec3.y = z * v.x - x * v.z;
	vec3.z = x * v.y - y * v.x;

	return vec3;
}

NVec3 NVec3::Lerp(const NVec3 start, const NVec3 end, const float timer)
{
	NVec3 distance = end - start;
	return distance * timer + start;
}

float NVec3::Dot(const NVec3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

NVec3 NVec3::operator+()const
{
	return *this;
}

NVec3 NVec3::operator-()const
{
	return NVec3(-x, -y, -z);
}

NVec3 NVec3::operator*(const NVec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

NVec3 NVec3::operator/(const NVec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

NVec3& NVec3::operator+=(const NVec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}

NVec3& NVec3::operator-=(const NVec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}

NVec3& NVec3::operator*=(const NVec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}

NVec3& NVec3::operator/=(const NVec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}

NVec3& NVec3::operator*=(const float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

NVec3& NVec3::operator/=(const float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

NVec3& NVec3::operator=(float num)
{
	x = num;
	y = num;
	z = num;

	return *this;
}

//NVec3ÉNÉâÉXÇ…ëÆÇ≥Ç»Ç¢ä÷êîåQ
// ìÒçÄââéZéq
const NVec3 operator+(const NVec3& v1, const NVec3& v2)
{
	NVec3 temp(v1);
	return temp += v2;
}

const NVec3 operator-(const NVec3& v1, const NVec3& v2)
{
	NVec3 temp(v1);
	return temp -= v2;
}

const NVec3 operator*(const NVec3& v, const float s)
{
	NVec3 temp(v);
	return temp *= s;
}

const NVec3 operator*(const float s, const NVec3& v)
{
	return v * s;
}

const NVec3 operator/(const NVec3& v, const float s)
{
	NVec3 temp(v);
	return temp /= s;
}