#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NObj3d.h"

//ここにあるやつはあくまで必要な要素の参考用で、
//それぞれNBaseColliderを継承した〇〇Colliderクラスを作る

//円コライダー//
struct Circle
{
	NVector2 centerPos;
	float radius;

	Circle();
	Circle(const NVector2& centerPos, const float radius);
};

//球コライダー//
struct Sphere
{
	NVector3 centerPos;
	float radius;

	Sphere();
	Sphere(const NVector3& centerPos, const float radius);
};

//レイコライダー//
struct Ray
{
	NVector3 startPos;
	NVector3 dirVec;

	Ray();
	Ray(const NVector3& startPos, const NVector3& dirVec);
};

//線コライダー//
struct Line
{
	NVector3 startPos;
	NVector3 endPos;

	Line();
	Line(const NVector3& startPos, const NVector3& endPos);
};

//平面コライダー//
struct Plane
{
	NVector3 normal;	//法線
	float distance;		//原点からの距離

	Plane();
	Plane(const NVector3& normal, const float distance);
};