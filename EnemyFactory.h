#pragma once
#include "EnemyManager.h"
#include "Wolf.h"

class EnemyFactory final
{
private:
	Wolf wolf;

	NVector3 offset;			//エミッター内でどれだけ出現位置ずらすか
	bool isCollision = false;	//敵同士が被ってるかフラグ

	uint32_t wolfSideNum = 10;				//狼群(横)の出現数
	NVector2 wolfSideEmitter = {2.0f,1.0f};	//狼群(横)の出現範囲

public:
	static EnemyFactory* GetInstance();

	void Update();
	//敵の生成
	void Create(IEnemy::EnemyType type, NVector3 pos);

private:
	EnemyFactory() = default;
	EnemyFactory(const EnemyFactory&) = delete;
	~EnemyFactory() = default;
	EnemyFactory& operator=(const EnemyFactory&) = delete;
};