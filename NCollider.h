#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NObj3d.h"

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

	std::unique_ptr<NObj3d> obj_;	//オブジェクト

	Sphere();
	Sphere(const NVector3& centerPos, const float radius);

	void Init();
	void Update();
	void Draw();
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