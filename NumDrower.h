#pragma once
#include "NSprite.h"

class NumDrower final
{
private:
	std::vector<std::unique_ptr<NSprite>> numSprite_;
	NVector2 pos_;					//座標保存用
	NVector2 size_;					//サイズ保存用
	NColor color_;					//色保存用
	std::vector<uint32_t> digit_;	//各桁の数字格納用
	const float scale_ = 64.f;		//文字の大きさ

public:
	//生成
	//spriteNum:確保するスプライトの数
	void Create(uint32_t spriteNum);
	void Update();
	void Draw();

	// ゲッター //
	NVector2 GetPos()const { return pos_; }
	NVector2 GetSize()const { return size_; }

	// セッター //
	void SetPos(const NVector2& pos);
	void SetSize(const NVector2& size);
	void SetColor(const NColor& color);
	void SetNum(uint32_t num);
};