#pragma once
#include "IPostEffect.h"

// ブルーム //

class Bloom :
	public IPostEffect
{
public:
	//初期化
	static void Init();
};