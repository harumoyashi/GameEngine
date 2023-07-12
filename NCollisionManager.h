#pragma once
#include <forward_list>

class NBaseCollider;

class NCollisionManager
{
private:
	std::forward_list<NBaseCollider*>colliders_;

	NBaseCollider* colA;
	NBaseCollider* colB;

public:
	static NCollisionManager* GetInstance();

	//コライダーをリストに追加
	inline void AddCollider(NBaseCollider* collider)
	{
		colliders_.emplace_front(collider);
	}
	//コライダーをリストから削除
	inline void RemoveCollider(NBaseCollider* collider)
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

	void SphereCol();
};