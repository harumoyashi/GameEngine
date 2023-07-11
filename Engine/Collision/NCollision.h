#pragma once
#include <DirectXMath.h>

#include "NCollider.h"

class NCollision
{
public:
	//‰~“¯Žm‚Ì“–‚½‚è”»’è
	static bool CircleCol(const Circle& c0, const Circle& c1);

	//‹…“¯Žm‚Ì“–‚½‚è”»’è
	static bool SphereCol(const Sphere& s0, const Sphere& s1);
	
	//‹…‚Æ•½–Ê‚Ì“–‚½‚è”»’è
	static bool Sphere2PlaneCol(const Sphere& sphere,const Plane& plane/*,NVector3 inter*/);
};