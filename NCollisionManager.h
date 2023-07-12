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

	//�R���C�_�[�����X�g�ɒǉ�
	inline void AddCollider(NBaseCollider* collider)
	{
		colliders_.emplace_front(collider);
	}
	//�R���C�_�[�����X�g����폜
	inline void RemoveCollider(NBaseCollider* collider)
	{
		colliders_.remove(collider);
	}
	//�S�Ă̏Փ˃`�F�b�N
	void CheckAllCollision();

private:
	NCollisionManager() = default;
	NCollisionManager(const NCollisionManager&) = delete;
	~NCollisionManager() = default;
	NCollisionManager& operator=(const NCollisionManager&) = delete;

	void SphereCol();
};