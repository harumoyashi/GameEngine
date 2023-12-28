#pragma once
#include "NSprite.h"

// 数字描画用 //

class NumDrower final
{
private:
	std::vector<std::unique_ptr<NSprite>> numSprite_;
	NVec2 pos_;						//座標保存用
	NVec2 size_;					//サイズ保存用
	NColor color_;					//色保存用
	std::vector<uint32_t> digit_;	//各桁の数字格納用
	const float scale_ = 64.f;		//文字の大きさ
	float indent_;					//数字同士の幅

public:
	//生成
	//spriteNum:確保するスプライトの数
	void Create(uint32_t spriteNum, float indent = 0.8f);
	//更新
	void Update();
	//描画
	void Draw();

	// ゲッター //
	//座標取得
	NVec2 GetPos()const { return pos_; }
	//大きさ取得
	NVec2 GetSize()const { return size_; }

	// セッター //
	//座標設定
	void SetPos(const NVec2& pos);
	//大きさ設定
	void SetSize(const NVec2& size);
	//インデント設定
	void SetIndent(float indent);
	//色設定
	void SetColor(const NColor& color);
	//表示する値設定
	void SetNum(uint32_t num);
};