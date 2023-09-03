#pragma once
#include <forward_list>

class NBaseCollider;

class NCollisionManager final
{
private:
	std::forward_list<NBaseCollider*>colliders_;

	NBaseCollider* colA_;
	NBaseCollider* colB_;

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

	void SphereCol();
	void Sphere2PlaneCol();
	void FieldCol();
};