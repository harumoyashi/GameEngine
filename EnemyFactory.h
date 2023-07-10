#pragma once
#include "EnemyManager.h"
#include "Wolf.h"

class EnemyFactory
{
private:
	Wolf wolf;

public:
	static EnemyFactory* GetInstance();

	void Update();
	//�G�̐���
	void Create(IEnemy::EnemyType type, NVector3 pos);
};