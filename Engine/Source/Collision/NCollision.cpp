#include "NCollision.h"

bool NCollision::CircleCol(const CircleCollider& c0, const CircleCollider& c1, const NVec2& inter)
{
	float distX = (c0.GetCenterPos().x - c1.GetCenterPos().x) * (c0.GetCenterPos().x - c1.GetCenterPos().x);
	float distY = (c0.GetCenterPos().y - c1.GetCenterPos().y) * (c0.GetCenterPos().y - c1.GetCenterPos().y);
	float distR = (c0.GetRadius() + c1.GetRadius()) * (c0.GetRadius() + c1.GetRadius());

	if (distX + distY <= distR)
	{
		return true;
	}
	return false;

	static_cast<void> (inter);	//引数をなかったことにする
}

bool NCollision::SquareCol(const SquareCollider& s0, const SquareCollider& s1, const NVec2& inter)
{
	float left0 = s0.GetCenterPos().x - s0.GetWide();
	float right0 = s0.GetCenterPos().x + s0.GetWide();
	float top0 = s0.GetCenterPos().y - s0.GetHeight();
	float bottom0 = s0.GetCenterPos().y + s0.GetHeight();

	float left1 = s1.GetCenterPos().x - s1.GetWide();
	float right1 = s1.GetCenterPos().x + s1.GetWide();
	float top1 = s1.GetCenterPos().y - s1.GetHeight();
	float bottom1 = s1.GetCenterPos().y + s1.GetHeight();

	if (left0 < right1 && right0 > left1)
	{
		if (top0 < bottom1 && bottom0 > top1)
		{
			return true;
		}
	}
	return false;

	static_cast<void> (inter);	//引数をなかったことにする
}

bool NCollision::Square2CircleCol(const SquareCollider& s, const CircleCollider& c, const NVec2& inter)
{
	float left = s.GetCenterPos().x - s.GetWide();
	float right = s.GetCenterPos().x + s.GetWide();
	float top = s.GetCenterPos().y - s.GetHeight();
	float bottom = s.GetCenterPos().y + s.GetHeight();

	float x = c.GetCenterPos().x;
	float y = c.GetCenterPos().y;
	float r = c.GetRadius();

	if (left - r > x || right + r < x || top - r > y || bottom + r < y) {	//矩形に円の半径分を足した範囲
		return false;
	}
	if (left > x && top > y && !((left - x) * (left - x) + (top - y) * (top - y) < r * r)) {                //左上の当たり判定
		return false;
	}
	if (right < x && top > y && !((right - x) * (right - x) + (top - y) * (top - y) < r * r)) {             //右上の当たり判定
		return false;
	}
	if (left > x && bottom < y && !((left - x) * (left - x) + (bottom - y) * (bottom - y) < r * r)) {       //左下の当たり判定
		return false;
	}
	if (right < x && bottom < y && !((right - x) * (right - x) + (bottom - y) * (bottom - y) < r * r)) {    //右下の当たり判定
		return false;
	}
	return true;//すべての条件が外れたときに当たっている

	static_cast<void> (inter);	//引数をなかったことにする
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

	static_cast<void> (inter);	//引数をなかったことにする
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
