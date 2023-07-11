#pragma once
#include "NBaseCollider.h"
#include "NCollider.h"

class SphereCollider:
	public NBaseCollider,public Sphere
{
private:
	NVector3 offset_;	//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	float radius_;		//���a

public:
	SphereCollider(NVector3 offset = NVector3(0, 0, 0), float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//�`��ݒ�
	}

	void Update()override;

	inline void SetRadius(float radius) { radius_ = radius; }
};