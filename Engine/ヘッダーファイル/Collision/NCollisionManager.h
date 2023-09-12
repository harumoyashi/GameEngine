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

	//������(�R���C�_�[�Q�S�폜)
	void Init();

	//�R���C�_�[�����X�g�ɒǉ�
	void AddCollider(NBaseCollider* collider)
	{
		colliders_.emplace_front(collider);
	}
	//�R���C�_�[�����X�g����폜
	void RemoveCollider(NBaseCollider* collider)
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

	//���̓��m�̓����蔻��
	void SphereCol();
	//���̂ƕ��ʂ̓����蔻��
	void Sphere2PlaneCol();
};