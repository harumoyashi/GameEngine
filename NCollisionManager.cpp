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
			colA_ = *itA;
			colB_ = *itB;

			//比較対象がある場合のみ判定を行う
			if (colA_ != nullptr && colB_ != nullptr)
			{
				SphereCol();
			}
		}
	}
}

void NCollisionManager::SphereCol()
{
	//球同士だった場合
	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE)
	{
		//型変換
		SphereCollider* sphereA = static_cast<SphereCollider*>(colA_);
		SphereCollider* sphereB = static_cast<SphereCollider*>(colB_);

		NVector3 inter; //交差点(今は使ってない)
		if (NCollision::SphereCol(*sphereA, *sphereB, inter))
		{
			colA_->SetIsCol(true);
			//コールバック関数ポインタの呼び出し
			//後ろの()には本来引数を入れるが、引数がないので空にしてる
			colA_->GetOnCollision()();
			colB_->SetIsCol(true);
			//コールバック関数ポインタの呼び出し
			colB_->GetOnCollision()();
		}
	}
}
