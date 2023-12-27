#pragma once
#include "IPostEffect.h"

// ガウシアンブラー //

class GaussianBlur final
	: public IPostEffect
{
public:
	//初期化
	static void Init();
};

