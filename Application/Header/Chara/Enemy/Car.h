#pragma once
#include "IEnemy.h"
#include "SquareCollider.h"

// 車の敵 //

class Car :
	public IEnemy
{
public:
	std::string modelname = "cube";   //モデル名
	SquareCollider collider_;		   //コライダー

	//コンストラクタ
	Car();

	// ゲッター //
	//コライダー取得
	const SquareCollider GetCollider()const { return collider_; }
};

