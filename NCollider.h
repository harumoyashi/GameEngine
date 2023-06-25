#pragma once
#include "NVector2.h"
#include "NVector3.h"

//�~�R���C�_�[//
struct CircleCollider
{
	NVector2 centerPos;
	float radius;

	CircleCollider();
	CircleCollider(const NVector2& centerPos, const float radius);
};

//���R���C�_�[//
struct SphereCollider
{
	NVector3 centerPos;
	float radius;

	SphereCollider();
	SphereCollider(const NVector3& centerPos, const float radius);
};

//���C�R���C�_�[//
struct ReyCollider
{
	NVector3 startPos;
	NVector3 dirVec;

	ReyCollider();
	ReyCollider(const NVector3& startPos, const NVector3& dirVec);
};

//���R���C�_�[//
struct LineCollider
{
	NVector3 startPos;
	NVector3 endPos;

	LineCollider();
	LineCollider(const NVector3& startPos, const NVector3& endPos);
};

//���ʃR���C�_�[//
struct PlaneCollider
{
	NVector3 normal;	//�@��
	float distance;		//���_����̋���

	PlaneCollider();
	PlaneCollider(const NVector3& normal, const float distance);
};