#pragma once
#include "IEnemy.h"

class EnemyManager final
{
private:
	bool isEnemyDead = false;

public:
	std::vector<std::unique_ptr<IEnemy>> enemys_;

	static EnemyManager* GetInstance();
	void Init();
	void Update();
	void Draw();
};