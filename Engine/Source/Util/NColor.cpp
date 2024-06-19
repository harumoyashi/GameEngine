#include "NColor.h"

const NColor NColor::kRed(1.0f, 0, 0);
const NColor NColor::kGreen(0, 1.0f, 0);
const NColor NColor::kBlue(0, 0, 1.0f);
const NColor NColor::kYellow(1.0f, 1.0f, 0);
const NColor NColor::kPink(1.0f, 0, 1.0f);
const NColor NColor::kLightblue(0, 1.0f, 1.0f);
const NColor NColor::kWhite(1.0f, 1.0f, 1.0f);
const NColor NColor::kBlack(0, 0, 0);
const NColor NColor::kPlayer(210.f / 225.f, 40.f / 225.f, 20.f / 225.f);
const NColor NColor::kEnemy(0.05f, 0.5f, 0.8f);
const NColor NColor::kItem(160.f / 225.f, 140.f / 225.f, 40.f / 225.f);


void NColor::SetColor255(const uint32_t R, const uint32_t G, const uint32_t B, const uint32_t A)
{
	r = static_cast<float>(R);
	g = static_cast<float>(G);
	b = static_cast<float>(B);
	a = static_cast<float>(A);
	Convert01();
}

void NColor::SetColor(const float R, const float G, const float B, const float A)
{
	r = R;
	g = G;
	b = B;
	a = A;
}

void NColor::SetColor(const NColor& col)
{
	r = col.r;
	g = col.g;
	b = col.b;
	a = col.a;
}

void NColor::Convert01()
{
	r /= 255.0f;
	g /= 255.0f;
	b /= 255.0f;
	a /= 255.0f;
}

void NColor::Convert255()
{
	r *= 255.0f;
	g *= 255.0f;
	b *= 255.0f;
	a *= 255.0f;
}

NColor& NColor::operator++()
{
	r++;
	g++;
	b++;
	return *this;
}

NColor NColor::operator+(const float num) const
{
	return { r + num, g + num, b + num };
}

NColor NColor::operator-(const float num) const
{
	return { r - num, g - num, b - num };
}

NColor NColor::operator*(const float num) const
{
	return { r * num, g * num, b * num };
}

NColor NColor::operator/(const float num) const
{
	return { r / num, g / num, b / num };
}

NColor NColor::operator=(const NColor& col)
{
	r = col.r, g = col.g, b = col.b, a = col.a;
	return *this;
}

bool NColor::operator==(const NColor& col) const
{
	if (r != col.r || g != col.g || b != col.b || a != col.a)
	{
		return false;
	}
	return true;
}

bool NColor::operator!=(const NColor& col) const
{
	if (r != col.r || g != col.g || b != col.b || a != col.a)
	{
		return true;
	}
	return false;
}

NColor NColor::operator+(const NColor& col) const
{
	NColor color;
	//両方の値を足して2で割ることで中間の色とする
	color.r = (r + col.r) * 0.5f;
	color.g = (g + col.g) * 0.5f;
	color.b = (b + col.b) * 0.5f;
	color.a = (a + col.a) * 0.5f;
	return color;
}
