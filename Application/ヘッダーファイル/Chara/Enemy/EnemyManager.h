#pragma once
#include "IEnemy.h"

class EnemyManager final
{
private:
	bool isEnemyDead = false;

public:
	std::vector<std::unique_ptr<IEnemy>> enemys_;

	NEasing::EaseTimer beatInTimer_ = 0.25f;		//���Y���ɏ��p�^�C�}�[(�ł����Ȃ�)
	NEasing::EaseTimer beatOutTimer_ = 0.25f;		//���Y���ɏ��p�^�C�}�[(�k��)

	static EnemyManager* GetInstance();
	void Init();
	void Update();
	void Draw();

private:
	EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	~EnemyManager() = default;
	EnemyManager& operator=(const EnemyManager&) = delete;
};