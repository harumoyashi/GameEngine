#pragma once
#include "IPostEffect.h"

// ガウシアンブラー //

class GaussianBlur final
	: public IPostEffect
{
public:
	static void Init();
};

