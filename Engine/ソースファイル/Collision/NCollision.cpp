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

bool NCollision::Sphere2PlaneCol(const SphereCollider& sphere, const PlaneCollider& plane)
{
	//座標系の原点から球の中心座標への距離
	float distV = sphere.GetCenterPos().Dot(plane.GetNormal());

	//平面の原点距離を減算することで、平面と球の中心との距離を出す
	float dist = distV - plane.GetDistance();

	//距離の絶対値が半径より大きければ当たってない
	if (fabsf(dist) > sphere.GetRadius())
	{
		return false;
	}

	return true;
}

bool NCollision::Ray2PlaneCol(const RayCollider& ray, const PlaneCollider& plane)
{
	const float epsilon = 1.0e-5f;	//誤差吸収用の極小の値
	//面法線とレイの方向ベクトルの内積
	float d1 = plane.GetNormal().Dot(ray.GetDirVec());
	//裏面には当たらない
	if (d1 > -epsilon)
	{
		return false;
	}
	//始点と原点の距離(平面の法線方向)
	//面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = plane.GetNormal().Dot(ray.GetStartPos());
	//始点と平面の距離(平面の法線方向)
	float dist = d2 - plane.GetDistance();
	//始点と平面の距離(レイ方向)
	float t = dist / -d1;
	//交点が始点より後ろにあるので、当たらない
	if (t < 0)
	{
		return false;
	}
	//交点を計算
	

	return true;
}
