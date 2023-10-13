#pragma once
#include "GaussianBlur.h"
#include "RadialBlur.h"
#include "Bloom.h"

class NPostEffectManager
{
private:
	std::unique_ptr<GaussianBlur> gaussian_;
	std::unique_ptr<RadialBlur> radial_;
	std::unique_ptr<Bloom> bloom_;

public:
	static NPostEffectManager* GetInstance();

	void Init();
	void Update();
	void PreDraw();
	void PostDraw();
};