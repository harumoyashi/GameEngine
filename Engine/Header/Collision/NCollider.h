#pragma once
#include "NVec2.h"
#include "NVec3.h"
#include "NObj3d.h"

//ここにあるやつはあくまで必要な要素の参考用で、
//それぞれNBaseColliderを継承した〇〇Colliderクラスを作る

//円コライダー//
struct Circle
{
	NVec2 centerPos;
	float radius;

	Circle();
	Circle(const NVec2& centerPos, const float radius);
};

//矩形コライダー//
struct Square
{
	NVec2 centerPos;
	float wide;
	float height;

	Square();
	Square(const NVec2& centerPos, const float wide, const float height);
};

//球コライダー//
struct Sphere
{
	NVec3 centerPos;
	float radius;

	Sphere();
	Sphere(const NVec3& centerPos, const float radius);
};

//レイコライダー//
struct Ray
{
	NVec3 startPos;
	NVec3 dirVec;

	Ray();
	Ray(const NVec3& startPos, const NVec3& dirVec);
};

//線コライダー//
struct Line
{
	NVec3 startPos;
	NVec3 endPos;

	Line();
	Line(const NVec3& startPos, const NVec3& endPos);
};

//平面コライダー//
struct Plane
{
	NVec3 normal;	//法線
	float distance;		//原点からの距離

	Plane();
	Plane(const NVec3& normal, const float distance);
};