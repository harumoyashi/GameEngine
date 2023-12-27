#pragma once
#include "GaussianBlur.h"
#include "RadialBlur.h"
#include "Bloom.h"

// ポストエフェクト管理 //

class NPostEffectManager
{
private:
	std::unique_ptr<GaussianBlur> gaussian_;
	std::unique_ptr<RadialBlur> radial_;
	std::unique_ptr<Bloom> bloom_;

public:
	static NPostEffectManager* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	void PreDraw();
	void PostDraw();
};