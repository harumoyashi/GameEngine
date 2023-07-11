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
			colA = *itA;
			colB = *itB;

			//��r�Ώۂ�����ꍇ�̂ݔ�����s��
			if (colA != nullptr && colB != nullptr)
			{
				SphereCol();
			}
		}
	}

}

void NCollisionManager::SphereCol()
{
	//�����m�������ꍇ
	if (colA->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE &&
		colB->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE)
	{
		//�^�ϊ�
		Sphere* sphereA = dynamic_cast<Sphere*>(colA);
		Sphere* sphereB = dynamic_cast<Sphere*>(colB);

		NVector3 inter; //�����_(���͎g���ĂȂ�)
		if (NCollision::SphereCol(*sphereA, *sphereB, inter))
		{
			colA->OnCollision(NCollisionInfo(colB, inter));
			colB->OnCollision(NCollisionInfo(colA, inter));
		}
	}
}
