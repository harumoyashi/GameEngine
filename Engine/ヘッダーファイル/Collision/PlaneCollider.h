#pragma once
#include "NBaseCollider.h"

class PlaneCollider final :
	public NBaseCollider
{
private:
	NVec3 normal_;		//�@��
	float distance_;	//���_����̋���

public:
	PlaneCollider(NVec3 normal = NVec3::up, float distance = 0.f) :
		normal_(normal), distance_(distance)
	{
		shapeType_ = COL_PLANE;
	}

	void Update(NObj3d* obj)override;

	// �Q�b�^�[ //
	//���_����̋����擾
	float GetDistance()const { return distance_; }
	//�@���擾
	const NVec3& GetNormal()const { return normal_; }

	// �Z�b�^�[ //
	//���_����̋����ݒ�
	void SetDistance(float distance) { distance_ = distance; }
	//�@���ݒ�
	void SetNormal(const NVec3& normal) { normal_ = normal; }
};

