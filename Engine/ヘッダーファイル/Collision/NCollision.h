#pragma once
#include <DirectXMath.h>

#include "NCollider.h"
#include "SphereCollider.h"

class NCollision final
{
public:
	//‰~“¯Žm‚Ì“–‚½‚è”»’è
	static bool CircleCol(const Circle& c0, const Circle& c1,const NVec2& inter = NVec2(0, 0));

	//‹…“¯Žm‚Ì“–‚½‚è”»’è
	static bool SphereCol(const SphereCollider& s0, const SphereCollider& s1, const NVec3& inter = NVec3(0, 0, 0));
	
	//‹…‚Æ•½–Ê‚Ì“–‚½‚è”»’è
	static bool Sphere2PlaneCol(const Sphere& sphere,const Plane& plane,NVec3 inter = NVec3(0,0,0));
};