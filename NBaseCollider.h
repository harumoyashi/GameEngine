#pragma once
#include "NObj3d.h"
#include "NCollisionInfo.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//����`
		COL_SPHERE,				//��
	};

protected:
	NObj3d* obj_ = nullptr;					//�I�u�W�F�N�g
	ColShapeType shapeType = SHAPE_UNKNOWN;	//�`��^�C�v

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update() = 0;

	inline void OnCollision(const NCollisionInfo& info)
	{
		obj_->OnCollision(info);
	}

	// �Q�b�^�[ //
	//�I�u�W�F�N�g�擾
	inline NObj3d* GetObj3d() { return obj_; }
	//�`��^�C�v�擾
	inline ColShapeType GetShapeType() { return shapeType; }

	// �Z�b�^�[ //
	//�I�u�W�F�N�g�ݒ�
	inline void SetObj(NObj3d* obj) { obj_ = obj; }
};

