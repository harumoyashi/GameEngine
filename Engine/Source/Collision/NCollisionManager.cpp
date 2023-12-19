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
					//CircleCol();
					SquareCol();
					Square2CircleCol();
					//SphereCol();
					//Sphere2PlaneCol();
				}
			}
		}
	}
}

void NCollisionManager::CircleCol()
{
	//円同士だった場合
	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_CIRCLE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_CIRCLE)
	{
		//型変換
		CircleCollider* circleA = static_cast<CircleCollider*>(colA_);
		CircleCollider* circleB = static_cast<CircleCollider*>(colB_);

		NVec2 inter; //交差点(今は使ってない)
		if (NCollision::CircleCol(*circleA, *circleB, inter))
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

void NCollisionManager::SquareCol()
{
	//矩形同士だった場合
	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_SQUARE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_SQUARE)
	{
		//型変換
		SquareCollider* squareA = static_cast<SquareCollider*>(colA_);
		SquareCollider* squareB = static_cast<SquareCollider*>(colB_);

		NVec2 inter; //交差点(今は使ってない)
		if (NCollision::SquareCol(*squareA, *squareB, inter))
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

void NCollisionManager::Square2CircleCol()
{
	//矩形と円か確かめる
	SquareCollider* square{};
	CircleCollider* circle{};

	if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_SQUARE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_CIRCLE)
	{
		//型変換
		square = static_cast<SquareCollider*>(colA_);
		circle = static_cast<CircleCollider*>(colB_);
	}
	else if (colA_->GetShapeType() == NBaseCollider::ColShapeType::COL_CIRCLE &&
		colB_->GetShapeType() == NBaseCollider::ColShapeType::COL_SQUARE)
	{
		//型変換
		square = static_cast<SquareCollider*>(colB_);
		circle = static_cast<CircleCollider*>(colA_);
	}
	else
	{
		return;		//矩形と円じゃないなら終わり
	}

	//矩形と円だった場合当たり判定を取る
	NVec2 inter; //交差点(今は使ってない)
	if (NCollision::Square2CircleCol(*square, *circle, inter))
	{
		colA_->SetColInfo(colB_);		//衝突相手のコライダーを登録
		colA_->GetOnCollision()();		//コールバック関数ポインタの呼び出し。後ろの()には本来引数を入れるが、引数がないので空にしてる。
		colA_->SetIsCol(true);			//いるかわからないが一応当たったよフラグtrueに

		colB_->SetColInfo(colA_);
		colB_->GetOnCollision()();
		colB_->SetIsCol(true);
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
