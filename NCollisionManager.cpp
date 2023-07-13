#include "NCollisionManager.h"
#include "NBaseCollider.h"
#include "NCollision.h"

NCollisionManager* NCollisionManager::GetInstance()
{
	static NCollisionManager instance;
	return &instance;
}

void NCollisionManager::CheckAllCollision()
{
	std::forward_list<NBaseCollider*>::iterator itA;
	std::forward_list<NBaseCollider*>::iterator itB;

	//�S�Ă̑g�ݍ��킹�ɂ��đ�������Ń`�F�b�N
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB)
		{
			colA_ = *itA;
			colB_ = *itB;

			//��r�Ώۂ�����ꍇ�̂ݔ�����s��
			if (colA_ != nullptr && colB_ != nullptr)
			{
				SphereCol();
			}
		}
	}
}

void NCollisionManager::SphereCol()
{
	//�����m�������ꍇ
	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE)
	{
		//�^�ϊ�
		SphereCollider* sphereA = static_cast<SphereCollider*>(colA_);
		SphereCollider* sphereB = static_cast<SphereCollider*>(colB_);

		NVector3 inter; //�����_(���͎g���ĂȂ�)
		if (NCollision::SphereCol(*sphereA, *sphereB, inter))
		{
			colA_->SetIsCol(true);
			//�R�[���o�b�N�֐��|�C���^�̌Ăяo��
			//����()�ɂ͖{�����������邪�A�������Ȃ��̂ŋ�ɂ��Ă�
			colA_->GetOnCollision()();
			colB_->SetIsCol(true);
			//�R�[���o�b�N�֐��|�C���^�̌Ăяo��
			colB_->GetOnCollision()();
		}
	}
}
