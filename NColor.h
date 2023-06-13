#pragma once
#include <stdint.h>

class NColor
{
public:
	float r, g, b, a;

	NColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	NColor(const float r, const float g, const float b) : r(r), g(g), b(b), a(1.0f) {}
	NColor(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) {}

	//�F���ύX(uint32_t�^0~255)
	void SetColor255(const uint32_t R = 255, const uint32_t G = 255, const uint32_t B = 255, const uint32_t A = 255);
	void SetColor(const float R = 1.0f, const float G = 1.0f, const float B = 1.0f, const float A = 1.0f);
	//0.0f~1.0f�ɒ���
	void Convert01();
	//0.0f~255.0f�ɒ���
	void Convert255();

	//�萔�ɂ́uk�v������(�h�C�c���konstant�R��)
	const static NColor kRed;
	const static NColor kGreen;
	const static NColor kBlue;
	const static NColor kYellow;
	const static NColor kPink;
	const static NColor kLightblue;
	const static NColor kWhite;
	const static NColor kBlack;

	NColor& operator++();

	//�P�����Z�q
	NColor operator+(const float num) const;
	NColor operator-(const float num) const;
	NColor operator*(const float num) const;
	NColor operator/(const float num) const;
	NColor operator=(const NColor& col);
};