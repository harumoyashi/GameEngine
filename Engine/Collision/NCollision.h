#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#include "NCollider.h"

class NCollision
{
public:
	//‰~“¯Žm‚Ì“–‚½‚è”»’è
	static bool CircleCol(const CircleCollider& c0, const CircleCollider& c1);

	//‹…“¯Žm‚Ì“–‚½‚è”»’è
	static bool SphereCol(const SphereCollider& s0, const SphereCollider& s1);
	
	//‹…‚Æ•½–Ê‚Ì“–‚½‚è”»’è
	static bool Sphere2PlaneCol(const SphereCollider& sphere,const PlaneCollider& plane/*,NVector3 inter*/);
};