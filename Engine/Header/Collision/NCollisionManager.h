#pragma once
#include <forward_list>

class NBaseCollider;

class NCollisionManager final
{
private:
	std::forward_list<NBaseCollider*>colliders_;

	NBaseCollider* colA_;
	NBaseCollider* colB_;

	bool isCollisionCheck = true;	//当たり判定するかフラグ

public:
	static NCollisionManager* GetInstance();

	//初期化(コライダー群全削除)
	void Init();

	//コライダーをリストに追加
	void AddCollider(NBaseCollider* collider)
	{
		colliders_.emplace_front(collider);
	}
	//コライダーをリストから削除
	void RemoveCollider(NBaseCollider* collider)
	{
		colliders_.remove(collider);
	}

	//全ての衝突チェック
	void CheckAllCollision();

private:
	NCollisionManager() = default;
	NCollisionManager(const NCollisionManager&) = delete;
	~NCollisionManager() = default;
	NCollisionManager& operator=(const NCollisionManager&) = delete;

	//球体同士の当たり判定
	void SphereCol();
	//球体と平面の当たり判定
	void Sphere2PlaneCol();
};