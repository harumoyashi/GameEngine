#pragma once
#include "IEnemy.h"

// 敵管理 //

class EnemyManager final
{
private:
	bool isEnemyDead = false;

public:
	std::vector<std::unique_ptr<IEnemy>> enemys_;

	NEasing::EaseTimer beatInTimer_ = 0.25f;		//リズムに乗る用タイマー(でかくなる)
	NEasing::EaseTimer beatOutTimer_ = 0.25f;		//リズムに乗る用タイマー(縮む)

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