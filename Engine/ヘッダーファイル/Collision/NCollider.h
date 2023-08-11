#pragma once
#include "NVec2.h"
#include "NVec3.h"
#include "NObj3d.h"

//�����ɂ����͂����܂ŕK�v�ȗv�f�̎Q�l�p�ŁA
//���ꂼ��NBaseCollider���p�������Z�ZCollider�N���X�����

//�~�R���C�_�[//
struct Circle
{
	NVec2 centerPos;
	float radius;

	Circle();
	Circle(const NVec2& centerPos, const float radius);
};

//���R���C�_�[//
struct Sphere
{
	NVec3 centerPos;
	float radius;

	Sphere();
	Sphere(const NVec3& centerPos, const float radius);
};

//���C�R���C�_�[//
struct Ray
{
	NVec3 startPos;
	NVec3 dirVec;

	Ray();
	Ray(const NVec3& startPos, const NVec3& dirVec);
};

//���R���C�_�[//
struct Line
{
	NVec3 startPos;
	NVec3 endPos;

	Line();
	Line(const NVec3& startPos, const NVec3& endPos);
};

//���ʃR���C�_�[//
struct Plane
{
	NVec3 normal;	//�@��
	float distance;		//���_����̋���

	Plane();
	Plane(const NVec3& normal, const float distance);
};