#include "NCollision.h"

bool NCollision::CircleCol(const Circle& c0, const Circle& c1, const NVec2& inter)
{
	float distX = (c0.centerPos.x - c1.centerPos.x) * (c0.centerPos.x - c1.centerPos.x);
	float distY = (c0.centerPos.y - c1.centerPos.y) * (c0.centerPos.y - c1.centerPos.y);
	float distR = (c0.radius + c1.radius) * (c0.radius + c1.radius);

	if (distX + distY <= distR)
	{
		return true;
	}
	return false;
}

bool NCollision::SphereCol(const SphereCollider& s0, const SphereCollider& s1, const NVec3& inter)
{
	float distX = (s0.GetCenterPos().x - s1.GetCenterPos().x) * (s0.GetCenterPos().x - s1.GetCenterPos().x);
	float distY = (s0.GetCenterPos().y - s1.GetCenterPos().y) * (s0.GetCenterPos().y - s1.GetCenterPos().y);
	float distZ = (s0.GetCenterPos().z - s1.GetCenterPos().z) * (s0.GetCenterPos().z - s1.GetCenterPos().z);
	float distR = (s0.GetRadius() + s1.GetRadius()) * (s0.GetRadius() + s1.GetRadius());

	if (distX + distY + distZ <= distR)
	{
		return true;
	}
	return false;
}

bool NCollision::Sphere2PlaneCol(const Sphere& sphere, const Plane& plane, NVec3 inter)
{
	//座標系の原点から球の中心座標への距離
	float distV = sphere.centerPos.Dot(plane.normal);

	//平面の原点距離を減算することで、平面と球の中心との距離を出す
	float dist = distV - plane.distance;

	//距離の絶対値が半径より大きければ当たってない
	if (fabsf(dist) > sphere.radius)
	{
		return false;
	}

	return true;
}