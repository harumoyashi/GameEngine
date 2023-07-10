#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NObj3d.h"

//円コライダー//
struct CircleCollider
{
	NVector2 centerPos;
	float radius;

	CircleCollider();
	CircleCollider(const NVector2& centerPos, const float radius);
};

//球コライダー//
struct SphereCollider
{
	NVector3 centerPos;
	float radius;

	std::unique_ptr<NObj3d> obj_;	//オブジェクト

	SphereCollider();
	SphereCollider(const NVector3& centerPos, const float radius);

	void Init();
	void Update();
	void Draw();
};

//レイコライダー//
struct ReyCollider
{
	NVector3 startPos;
	NVector3 dirVec;

	ReyCollider();
	ReyCollider(const NVector3& startPos, const NVector3& dirVec);
};

//線コライダー//
struct LineCollider
{
	NVector3 startPos;
	NVector3 endPos;

	LineCollider();
	LineCollider(const NVector3& startPos, const NVector3& endPos);
};

//平面コライダー//
struct PlaneCollider
{
	NVector3 normal;	//法線
	float distance;		//原点からの距離

	PlaneCollider();
	PlaneCollider(const NVector3& normal, const float distance);
};