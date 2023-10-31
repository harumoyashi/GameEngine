#include "NCollisionManager.h"
#include "NBaseCollider.h"
#include "NCollision.h"

#include "NImGuiManager.h"
#include "imgui.h"

NCollisionManager* NCollisionManager::GetInstance()
{
	static NCollisionManager instance;
	return &instance;
}

void NCollisionManager::Init()
{
	colliders_.clear();
}

void NCollisionManager::CheckAllCollision()
{
	std::forward_list<NBaseCollider*>::iterator itA;
	std::forward_list<NBaseCollider*>::iterator itB;

#ifdef _DEBUG
	ImGui::Begin("Collision");
	ImGui::Checkbox("IsCollisionCheck", &isCollisionCheck);	//当たり判定するかフラグ指定
	ImGui::End();
#endif // _DEBUG

	if (isCollisionCheck)
	{
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
					//Sphere2PlaneCol();
				}
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

		NVec3 inter; //交差点(今は使ってない)
		if (NCollision::SphereCol(*sphereA, *sphereB, inter))
		{
			colA_->SetColInfo(colB_);		//衝突相手のコライダーを登録
			colA_->GetOnCollision()();		//コールバック関数ポインタの呼び出し。後ろの()には本来引数を入れるが、引数がないので空にしてる。
			colA_->SetIsCol(true);			//いるかわからないが一応当たったよフラグtrueに

			colB_->SetColInfo(colA_);
			colB_->GetOnCollision()();
			colB_->SetIsCol(true);
		}
	}
}

void NCollisionManager::Sphere2PlaneCol()
{
	//球と平面か確かめる
	SphereCollider* sphere{};
	PlaneCollider* plane{};

	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_PLANE)
	{
		//型変換
		sphere = static_cast<SphereCollider*>(colA_);
		plane = static_cast<PlaneCollider*>(colB_);
	}
	else if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_PLANE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_SPHERE)
	{
		//型変換
		sphere = static_cast<SphereCollider*>(colB_);
		plane = static_cast<PlaneCollider*>(colA_);
	}
	else
	{
		return;		//球と平面じゃないなら終わり
	}

	//球と平面だった場合当たり判定を取る
	if (NCollision::Sphere2PlaneCol(*sphere, *plane))
	{
		colA_->SetColInfo(colB_);		//衝突相手のコライダーを登録
		colA_->GetOnCollision()();		//コールバック関数ポインタの呼び出し。後ろの()には本来引数を入れるが、引数がないので空にしてる。
		colA_->SetIsCol(true);			//いるかわからないが一応当たったよフラグtrueに

		colB_->SetColInfo(colA_);
		colB_->GetOnCollision()();
		colB_->SetIsCol(true);
	}
}
