#pragma once
#include "NBaseCollider.h"

class SphereCollider final :
	public NBaseCollider
{
private:
	NVec3 centerPos_;	//�R���C�_�[�̒��S���W
	NVec3 offset_;		//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	float radius_;		//���a

public:
	SphereCollider(NVec3 offset = NVec3::zero, float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//�`��ݒ�
	}

	void Update(NObj3d* obj)override;

	// �Q�b�^�[ //
	//���a�擾
	float GetRadius()const { return radius_; }
	//�R���C�_�[�̒��S���W�擾
	const NVec3& GetCenterPos()const { return centerPos_; }

	// �Z�b�^�[ //
	//���a�ݒ�
	void SetRadius(float radius) { radius_ = radius; }
	//�R���C�_�[�̒��S���W�ݒ�
	void SetCenterPos(NVec3 centerPos) { centerPos_ = centerPos; }
};