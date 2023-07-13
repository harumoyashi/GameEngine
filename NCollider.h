#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NObj3d.h"

//�����ɂ����͂����܂ŕK�v�ȗv�f�̎Q�l�p�ŁA
//���ꂼ��NBaseCollider���p�������Z�ZCollider�N���X�����

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

	Sphere();
	Sphere(const NVector3& centerPos, const float radius);
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