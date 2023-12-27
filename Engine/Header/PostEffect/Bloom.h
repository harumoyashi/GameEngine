#pragma once
#include "IPostEffect.h"

// ブルーム //

class Bloom :
	public IPostEffect
{
public:
	static void Init();
};