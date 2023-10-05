#include "NQuaternion.h"
#include "NMatrix4.h"
#include <math.h>

NQuaternion NQuaternion::operator+(const NQuaternion& q) const
{
	return NQuaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

NQuaternion NQuaternion::operator-(const NQuaternion& q) const
{
	return NQuaternion(x - q.x, y - q.y, z - q.z, w - q.w);
}

NQuaternion NQuaternion::operator*(const NQuaternion& q) const
{
	return NQuaternion(
		w * q.x - z * q.y + y * q.z + x * q.w,
		z * q.x + w * q.y - x * q.z + y * q.w,
		-y * q.x + x * q.y + w * q.z + z * q.w,
		-x * q.x - y * q.y - z * q.z + w * q.w
	);
}

NQuaternion NQuaternion::operator+=(const NQuaternion& q)
{
	NQuaternion result = *this;
	result = result + q;
	*this = result;
	return *this;
}

NQuaternion NQuaternion::operator-=(const NQuaternion& q)
{
	NQuaternion result = *this;
	result = result - q;
	*this = result;
	return *this;
}

NQuaternion& NQuaternion::operator*=(const NQuaternion& q)
{
	NQuaternion result = *this;
	result = result * q;
	*this = result;
	return *this;
}

NQuaternion NQuaternion::operator-() const
{
	return NQuaternion{ -x,-y,-z,-w };
}

NQuaternion NQuaternion::operator*(const float s) const
{
	return NQuaternion(x * s, y * s, z * s, w * s);
}

NQuaternion NQuaternion::operator/(const float s) const
{
	return NQuaternion(x / s, y / s, z / s, w / s);
}

NQuaternion& NQuaternion::operator*=(const float s)
{
	NQuaternion result = *this;
	result = result * s;
	*this = result;
	return *this;
}

NQuaternion& NQuaternion::operator/=(const float s)
{
	NQuaternion result = *this;
	result = result / s;
	*this = result;
	return *this;
}

NQuaternion NQuaternion::Mul(const NQuaternion& lhs, const NQuaternion& rhs)
{
	NQuaternion lq = lhs;
	NQuaternion rq = rhs;

	NQuaternion result(
		lq.w * rq.x - lq.z * rq.y + lq.y * rq.z + lq.x * rq.w,
		lq.z * rq.x + lq.w * rq.y - lq.x * rq.z + lq.y * rq.w,
		-lq.y * rq.x + lq.x * rq.y + lq.w * rq.z + lq.z * rq.w,
		-lq.x * rq.x - lq.y * rq.y - lq.z * rq.z + lq.w * rq.w
	);
	return result;
}

NQuaternion NQuaternion::Identity()
{
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->w = 1.f;
	return *this;
}

NQuaternion NQuaternion::Conjugate(const NQuaternion& q)
{
	NQuaternion result = q;
	//虚部を全て反転
	result.x = -result.x;
	result.y = -result.y;
	result.z = -result.z;
	return result;
}

float NQuaternion::GetNorm() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

float NQuaternion::Norm(const NQuaternion& q)
{
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

float NQuaternion::Dot(const NQuaternion& q0, const NQuaternion& q1)
{
	return q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
}

NVec3 NQuaternion::Cross(const NQuaternion& q0, const NQuaternion& q1)
{
	float crossX = q0.y * q1.z - q0.z * q1.y;
	float crossY = q0.z * q1.x - q0.x * q1.z;
	float crossZ = q0.x * q1.y - q0.y * q1.x;

	return { crossX,crossY,crossZ };
}

NQuaternion NQuaternion::Normalize()
{
	float norm = Norm(*this);
	*this /= norm;
	return *this;
}

NQuaternion NQuaternion::Normalize(const NQuaternion& q)
{
	NQuaternion result = q;
	float norm = Norm(result);
	result /= norm;
	return result;
}

NQuaternion NQuaternion::Inverse(const NQuaternion& q)
{
	NQuaternion result = q;
	float norm = Norm(result);
	result = Conjugate(result);	//共役を
	result /= (norm * norm);	//normの2乗(スカラー)で割ると逆クォータニオン
	return result;
}

NQuaternion NQuaternion::Lerp(const NQuaternion q1, const NQuaternion q2, float t)
{
	float cos = Dot(q1, q2);
	NQuaternion t2 = q2;

	if (cos < 0.f)
	{
		cos = -cos;
		t2 = -q2;
	}

	float k0 = 1.f - t;
	float k1 = t;

	if ((1.f - cos) > 0.001f)
	{
		float theta = acosf(cos);
		k0 = sinf(theta * k0) / sinf(theta);
		k1 = sinf(theta * k1) / sinf(theta);
	}

	if (cos >= 1.f - 0.0005f)
	{
		return  q1 * (1.f - t) + t2 * t;
	}

	return q1 * k0 + t2 * k1;
}

NQuaternion NQuaternion::MakeAxisAngle(const NVec3& axis, const float angle)
{
	NQuaternion q(
		axis.x * sinf(angle / 2.0f),
		axis.y * sinf(angle / 2.0f),
		axis.z * sinf(angle / 2.0f),
		cosf(angle / 2.0f)
	);
	q.Normalize();
	return q;
}

NQuaternion NQuaternion::EulerToQuaternion(const NVec3& rot) const
{
	NQuaternion angleX = MakeAxisAngle({ 1.f, 0.f, 0.f }, rot.x);
	NQuaternion angleY = MakeAxisAngle({ 0.f, 1.f, 0.f }, rot.y);
	NQuaternion angleZ = MakeAxisAngle({ 0.f, 0.f, 1.f }, rot.z);

	NQuaternion q = angleZ * angleX * angleY;

	return q;
}

NMatrix4 NQuaternion::QuaternionToMatrix() const
{
	NMatrix4 mat;
	mat = NMatrix4::Identity();

	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float ww = w * w;
	float xy2 = x * y * 2;
	float xz2 = x * z * 2;
	float xw2 = x * w * 2;
	float yz2 = y * z * 2;
	float yw2 = y * w * 2;
	float zw2 = z * w * 2;

	mat.m[0][0] = ww + xx - yy - zz;
	mat.m[0][1] = xy2 + zw2;
	mat.m[0][2] = xz2 - yw2;
	mat.m[1][0] = xy2 - zw2;
	mat.m[1][1] = ww - xx + yy - zz;
	mat.m[1][2] = yz2 + xw2;
	mat.m[2][0] = xz2 + yw2;
	mat.m[2][1] = yz2 - xw2;
	mat.m[2][2] = ww - xx - yy + zz;
	return mat;
}

NVec3 NQuaternion::RotateVector(const NVec3& vector, const NQuaternion& quaternion) const
{
	NMatrix4 mat;
	mat = quaternion.QuaternionToMatrix();

	NVec3 vec;
	vec = mat.ToEuler();

	NMatrix4 mat2;
	mat2 *= mat2.RotateZ(vec.z);
	mat2 *= mat2.RotateX(vec.x);
	mat2 *= mat2.RotateY(vec.y);

	return vector * mat;
}

NQuaternion NQuaternion::Slarp(const NQuaternion& q0, const NQuaternion& q1, const float t)
{
	float dot = Dot(q0, q1);
	NQuaternion q = q0;
	if (dot < 0)
	{
		q = -q;
		dot = -dot;
	}

	float scale0 = 1.0f - t;
	float scale1 = t;
	if ((1.0f - dot) > 0.001f)
	{
		float theta = acosf(dot);
		scale0 = sinf(theta * scale0) / sinf(theta);
		scale1 = sinf(theta * scale1) / sinf(theta);
	}

	if (dot >= 1.0f - 0.00001f)
	{
		return q * scale0 + q1 * scale1;
	}
	return q * (1.0f - t) + q1 * t;
}

NQuaternion NQuaternion::VecToVecRota(const NVec3& u, const NVec3& v)
{
	//正規化して内積を求める
	float dot = u.Normalize().Dot(v.Normalize());

	//軸を求める(外積の結果が単位ベクトルとは限らないので正規化必須)
	NVec3 axis = u.Cross(v).Normalize();

	//単位ベクトルで内積を取ったのでacosで角度を求める
	float theta = acosf(dot);

	//求めた軸と角度を使って任意軸回転を返す
	return MakeAxisAngle(axis, theta);
}
