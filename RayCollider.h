#pragma once
#include "NBaseCollider.h"

class RayCollider final :
	public NBaseCollider
{
private:
	NVec3 startPos_;
	NVec3 dirVec_;

public:
	RayCollider(NVec3 startPos = NVec3::zero, NVec3 dirVec = NVec3::zero) :
		startPos_(startPos), dirVec_(dirVec)
	{
		shapeType_ = COL_RAY;
	}

	void Update(NObj3d* obj)override;

	// �Q�b�^�[ //
	//���_����̋����擾
	const NVec3& GetStartPos()const { return startPos_; }
	//�@���擾
	const NVec3& GetDirVec()const { return dirVec_; }

	// �Z�b�^�[ //
	//���_����̋����ݒ�
	void SetStartPos(const NVec3& startPos) { startPos_ = startPos; }
	//�@���ݒ�
	void SetDirVec(const NVec3& dirVec) { dirVec_ = dirVec; }
};

