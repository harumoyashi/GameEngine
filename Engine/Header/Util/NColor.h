#pragma once
#include "NVec3.h"
#include <stdint.h>

struct NColor
{
	float r, g, b, a;

	NColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	NColor(const float r, const float g, const float b) : r(r), g(g), b(b), a(1.0f) {}
	NColor(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) {}

	//RGBだけVec3型で返す
	NVec3 GetRGB(){ return NVec3(r, g, b); }

	//色情報変更(uint32_t型0~255)
	void SetColor255(const uint32_t R = 255, const uint32_t G = 255, const uint32_t B = 255, const uint32_t A = 255);
	void SetColor(const float R = 1.0f, const float G = 1.0f, const float B = 1.0f, const float A = 1.0f);
	void SetColor(const NColor& col);
	//0.0f~1.0fに直す
	void Convert01();
	//0.0f~255.0fに直す
	void Convert255();

	//定数には「k」をつける(ドイツ語のkonstant由来)
	const static NColor kRed;
	const static NColor kGreen;
	const static NColor kBlue;
	const static NColor kYellow;
	const static NColor kPink;
	const static NColor kLightblue;
	const static NColor kWhite;
	const static NColor kBlack;
	const static NColor kPlayer;
	const static NColor kEnemy;
	const static NColor kItem;

	NColor& operator++();

	//単項演算子
	NColor operator+(const float num) const;
	NColor operator-(const float num) const;
	NColor operator*(const float num) const;
	NColor operator/(const float num) const;
	NColor operator=(const NColor& col);
	bool operator==(const NColor& col) const;
	bool operator!=(const NColor& col) const;
	NColor operator+(const NColor& col) const;
};