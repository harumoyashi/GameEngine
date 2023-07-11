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

	//全ての組み合わせについて総当たりでチェック
	itA = colliders_.begin();
	for (; itA != colliders_.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders_.end(); ++itB)
		{
			colA = *itA;
			colB = *itB;

			//比較対象がある場合のみ判定を行う
			if (colA != nullptr && colB != nullptr)
			{
				SphereCol();
			}
		}
	}

}

void NCollisionManager::SphereCol()
{
	//球同士だった場合
	if (colA->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE &&
		colB->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE)
	{
		//型変換
		Sphere* sphereA = dynamic_cast<Sphere*>(colA);
		Sphere* sphereB = dynamic_cast<Sphere*>(colB);

		NVector3 inter; //交差点(今は使ってない)
		if (NCollision::SphereCol(*sphereA, *sphereB, inter))
		{
			colA->OnCollision(NCollisionInfo(colB, inter));
			colB->OnCollision(NCollisionInfo(colA, inter));
		}
	}
}
