#pragma once
#include "IPostEffect.h"

// ラジアルブラー //

class RadialBlur final
	: public IPostEffect
{
public:
	//初期化
	static void Init();
};