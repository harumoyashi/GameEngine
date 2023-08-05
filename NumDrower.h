#pragma once
#include "NSprite.h"

class NumDrower
{
private:
	static std::vector<std::unique_ptr<NSprite>> numSprite_;
	static NVector2 pos_;					//座標保存用
	static NVector2 size_;					//サイズ保存用
	static std::vector<uint32_t> digit_;	//各桁の数字格納用
	static const float scale_;				//文字の大きさ

public:
	//生成
	//spriteNum:確保するスプライトの数
	static void Create(uint32_t spriteNum);
	static void Update();
	static void Draw();

	static void SetPos(const NVector2& pos);
	static void SetSize(const NVector2& size);
	static void SetNum(uint32_t num);
};