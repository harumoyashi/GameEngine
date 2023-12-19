#pragma once
#include <DirectXMath.h>

#include "NCollider.h"
#include "CircleCollider.h"
#include "SquareCollider.h"
#include "SphereCollider.h"
#include "PlaneCollider.h"
#include "RayCollider.h"

class NCollision final
{
public:
	//円同士の当たり判定
	static bool CircleCol(const CircleCollider& c0, const CircleCollider& c1, const NVec2& inter = NVec2(0, 0));

	//矩形同士の当たり判定
	static bool SquareCol(const SquareCollider& s0, const SquareCollider& s1, const NVec2& inter = NVec2(0, 0));

	//矩形と円の当たり判定
	static bool Square2CircleCol(const SquareCollider& s, const CircleCollider& c, const NVec2& inter = NVec2(0, 0));

	//球同士の当たり判定
	static bool SphereCol(const SphereCollider& s0, const SphereCollider& s1, const NVec3& inter = NVec3(0, 0, 0));

	//球と平面の当たり判定
	static bool Sphere2PlaneCol(const SphereCollider& sphere, const PlaneCollider& plane);

	//レイと平面の当たり判定
	static bool Ray2PlaneCol(const RayCollider& ray, const PlaneCollider& plane);
};