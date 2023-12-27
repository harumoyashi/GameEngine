#pragma once
#include "EnemyManager.h"
#include "Mouse.h"
#include "Snake.h"

// 敵生成工場 //

class EnemyFactory final
{
private:
	Mouse mouse;
	Snake snake;

	NVec3 offset;				//エミッター内でどれだけ出現位置ずらすか
	bool isCollision = false;	//敵同士が被ってるかフラグ

	uint32_t mouseSideNum = 10;				//ねずみ群(横)の出現数
	NVec2 mouseSideEmitter = { 2.0f,1.0f };	//ねずみ群(横)の出現範囲

	uint32_t mouseFrontNum = 8;				//ねずみ群(正面)の出現数
	NVec2 mouseFrontEmitter = { 6.0f,1.0f };//ねずみ群(正面)の出現範囲

	uint32_t snakeSideNum = 5;				//へび群(横)の出現数
	NVec2 snakeSideEmitter = { 2.0f,1.0f };	//へび群(横)の出現範囲

public:
	static EnemyFactory* GetInstance();

	void Update();
	//敵の生成
	//type:敵の種類
	//pos:生成の基準座標
	//isItem:アイテムを持った敵を生成するか
	void Create(IEnemy::EnemyType type, NVec3 pos, bool isItem);

private:
	EnemyFactory() = default;
	EnemyFactory(const EnemyFactory&) = delete;
	~EnemyFactory() = default;
	EnemyFactory& operator=(const EnemyFactory&) = delete;
};