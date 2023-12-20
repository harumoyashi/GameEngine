#pragma once
#include "IEnemy.h"
#include "SquareCollider.h"

class Snake :
	public IEnemy
{
public:
	std::string modelname = "snake";   //モデル名
	SquareCollider collider_;		   //コライダー

	Snake();

	// ゲッター //
	//コライダー取得
	const SquareCollider GetCollider()const { return collider_; }
};