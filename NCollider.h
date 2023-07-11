#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NObj3d.h"

//�~�R���C�_�[//
struct Circle
{
	NVector2 centerPos;
	float radius;

	Circle();
	Circle(const NVector2& centerPos, const float radius);
};

//���R���C�_�[//
struct Sphere
{
	NVector3 centerPos;
	float radius;

	std::unique_ptr<NObj3d> obj_;	//�I�u�W�F�N�g

	Sphere();
	Sphere(const NVector3& centerPos, const float radius);

	void Init();
	void Update();
	void Draw();
};

//���C�R���C�_�[//
struct Ray
{
	NVector3 startPos;
	NVector3 dirVec;

	Ray();
	Ray(const NVector3& startPos, const NVector3& dirVec);
};

//���R���C�_�[//
struct Line
{
	NVector3 startPos;
	NVector3 endPos;

	Line();
	Line(const NVector3& startPos, const NVector3& endPos);
};

//���ʃR���C�_�[//
struct Plane
{
	NVector3 normal;	//�@��
	float distance;		//���_����̋���

	Plane();
	Plane(const NVector3& normal, const float distance);
};