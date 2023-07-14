#pragma once
#include "EnemyManager.h"
#include "Wolf.h"

class EnemyFactory final
{
private:
	Wolf wolf;

	bool isCollision = false;
	uint32_t wolfSideNum = 10;

public:
	static EnemyFactory* GetInstance();

	void Update();
	//�G�̐���
	void Create(IEnemy::EnemyType type, NVector3 pos);
};