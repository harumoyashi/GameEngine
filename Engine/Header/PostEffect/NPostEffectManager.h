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
	//シングルトンインスタンス取得
	static NPostEffectManager* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
};