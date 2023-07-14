#pragma once
#include "NObj3d.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//����`
		COL_SPHERE,				//��
	};

protected:
	NObj3d* obj_ = nullptr;						//�I�u�W�F�N�g
	ColShapeType shapeType_ = SHAPE_UNKNOWN;	//�`��^�C�v
	bool isCollision_;							//�����������t���O

	NBaseCollider* colInfo_;					//�Փˑ���̃R���C�_�[
	std::string colID;							//�R���C�_�[�̎���ID
	NVector3 inter_;							//�Փ˓_

	std::function<void(void)>onCollision_;		//�R�[���o�b�N�֐��|�C���^�ϐ�

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update(NObj3d* obj) = 0;

	// �Q�b�^�[ //
	//�I�u�W�F�N�g�擾
	inline NObj3d* GetObj3d() { return obj_; }
	//�`��^�C�v�擾
	inline ColShapeType GetShapeType() { return shapeType_; }
	//�������Ă邩�t���O�擾
	inline bool GetIsCol() { return isCollision_; }
	//�R���C�_�[���g���擾
	inline NBaseCollider* GetCollider() { return this; }

public:
	// �Q�b�^�[ //
	//�R�[���o�b�N�֐��|�C���^�ϐ��擾
	inline std::function<void(void)> GetOnCollision()const { return onCollision_; }
	//�R���C�_�[�̎���ID�擾
	inline std::string GetColID()const { return colID; }
	//�Փˑ���̎擾
	inline NBaseCollider* GetColInfo()const { return colInfo_; }

	// �Z�b�^�[ //
	//�I�u�W�F�N�g�ݒ�
	inline void SetObj(NObj3d* obj) { obj_ = obj; }
	//�������Ă邩�t���O�ݒ�
	inline void SetIsCol(bool isCol) { isCollision_ = isCol; }
	//�R�[���o�b�N�֐��|�C���^�ϐ��ݒ�
	inline void SetOnCollision(const std::function<void(void)>& onCollision) { onCollision_ = onCollision; }
	//�R���C�_�[�̎���ID�ݒ�
	inline void SetColID(std::string id) { colID = id; }
	//�Փˑ���̐ݒ�
	inline void SetColInfo(NBaseCollider* colInfo) { colInfo_ = colInfo; }
};