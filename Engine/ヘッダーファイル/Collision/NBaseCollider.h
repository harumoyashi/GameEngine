#pragma once
#include "NObj3d.h"

class NBaseCollider
{
public:
	enum ColShapeType
	{
		SHAPE_UNKNOWN = -1,		//����`
		COL_SPHERE,				//��
		COL_PLANE,				//����
	};

	//�Փˑ�����`
	const unsigned short COL_TYPE_FIELD = 0b1 << 0;		//�n�`
	const unsigned short COL_TYPE_PLAYER = 0b1 << 1;	//�v���C���[�܂��
	const unsigned short COL_TYPE_ENEMY = 0b1 << 2;		//�G�܂��
	const unsigned short COL_TYPE_OBJECT = 0b1 << 3;	//�I�u�W�F�N�g�܂��

protected:
	NObj3d* obj_ = nullptr;							//�I�u�W�F�N�g
	ColShapeType shapeType_ = SHAPE_UNKNOWN;		//�`��^�C�v
	bool isCollision_;								//�����������t���O

	NBaseCollider* colInfo_;						//�Փˑ���̃R���C�_�[
	std::string colID;								//�R���C�_�[�̎���ID
	NVec3 inter_;									//�Փ˓_
	unsigned short colType_ = 0b0000000000000000;	//�Փˑ���

	std::function<void(void)>onCollision_;			//�R�[���o�b�N�֐��|�C���^�ϐ�

public:
	NBaseCollider() = default;
	virtual ~NBaseCollider() = default;

	virtual void Update(NObj3d* obj) = 0;

	// �Q�b�^�[ //
	//�I�u�W�F�N�g�擾
	NObj3d* GetObj3d() { return obj_; }
	//�`��^�C�v�擾
	ColShapeType GetShapeType() { return shapeType_; }
	//�������Ă邩�t���O�擾
	bool GetIsCol() { return isCollision_; }
	//�R���C�_�[���g���擾
	NBaseCollider* GetCollider() { return this; }

public:
	// �Q�b�^�[ //
	//�R�[���o�b�N�֐��|�C���^�ϐ��擾
	std::function<void(void)> GetOnCollision()const { return onCollision_; }
	//�R���C�_�[�̎���ID�擾
	std::string GetColID()const { return colID; }
	//�Փˑ���̎擾
	NBaseCollider* GetColInfo()const { return colInfo_; }

	// �Z�b�^�[ //
	//�I�u�W�F�N�g�ݒ�
	void SetObj(NObj3d* obj) { obj_ = obj; }
	//�������Ă邩�t���O�ݒ�
	void SetIsCol(bool isCol) { isCollision_ = isCol; }
	//�R�[���o�b�N�֐��|�C���^�ϐ��ݒ�
	void SetOnCollision(const std::function<void(void)>& onCollision) { onCollision_ = onCollision; }
	//�R���C�_�[�̎���ID�ݒ�
	void SetColID(std::string id) { colID = id; }
	//�Փˑ���̐ݒ�
	void SetColInfo(NBaseCollider* colInfo) { colInfo_ = colInfo; }
	//�Փˑ����̐ݒ�
	void SetColType(unsigned short colType) { colType_ = colType; }
	//�Փˑ����̒ǉ�
	void AddColType(unsigned short colType) { colType_ |= colType; }
	//�Փˑ����̍폜
	void RemoveColType(unsigned short colType) { colType_ &= !colType; }
};