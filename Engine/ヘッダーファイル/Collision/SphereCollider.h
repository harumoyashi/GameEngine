#pragma once
#include "NBaseCollider.h"
#include "NCollider.h"

class SphereCollider final :
	public NBaseCollider
{
private:
	NVector3 centerPos_;	//�R���C�_�[�̒��S���W
	NVector3 offset_;		//�I�u�W�F�N�g���S����̃I�t�Z�b�g
	float radius_;			//���a

public:
	SphereCollider(NVector3 offset = NVector3(0, 0, 0), float radius = 1.0f) :
		offset_(offset), radius_(radius)
	{
		shapeType_ = COL_SPHERE;	//�`��ݒ�
	}

	void Update(NObj3d* obj)override;

	// �Q�b�^�[ //
	//���a�ݒ�
	float GetRadius()const { return radius_; }
	//�R���C�_�[�̒��S���W�擾
	const NVector3& GetCenterPos()const { return centerPos_; }

	// �Z�b�^�[ //
	//���a�ݒ�
	void SetRadius(float radius) { radius_ = radius; }
	//�R���C�_�[�̒��S���W�ݒ�
	void SetCenterPos(NVector3 centerPos) { centerPos_ = centerPos; }
};