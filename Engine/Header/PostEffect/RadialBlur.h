#pragma once
#include "IPostEffect.h"

// ラジアルブラー //

class RadialBlur final
	: public IPostEffect
{
public:
	static void Init();
};