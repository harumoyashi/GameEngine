#pragma once
#include "IEnemy.h"
#include "SquareCollider.h"

// ヘビの敵 //

class Snake :
	public IEnemy
{
public:
	std::string modelname = "snake";   //モデル名
	SquareCollider collider_;		   //コライダー

	//コンストラクタ
	Snake();

	// ゲッター //
	//コライダー取得
	const SquareCollider GetCollider()const { return collider_; }
};