#include "NColor.h"

const NColor NColor::red(1.0f, 0, 0);
const NColor NColor::green(0, 1.0f, 0);
const NColor NColor::blue(0, 0, 1.0f);
const NColor NColor::yellow(1.0f, 1.0f, 0);
const NColor NColor::pink(1.0f, 0, 1.0f);
const NColor NColor::lightblue(0, 1.0f, 1.0f);
const NColor NColor::white(1.0f, 1.0f, 1.0f);
const NColor NColor::black(0, 0, 0);

void NColor::SetColor255(uint32_t R, uint32_t G, uint32_t B, uint32_t A)
{
	r = static_cast<float>(R);
	g = static_cast<float>(G);
	b = static_cast<float>(B);
	a = static_cast<float>(A);
	Convert01();
}

void NColor::SetColor(float R , float G, float B , float A )
{
	r = R;
	g = G;
	b = B;
	a = A;
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

NColor NColor::operator+(float num) const
{
	return { r + num, g + num, b + num };
}

NColor NColor::operator-(float num) const
{
	return { r - num, g - num, b - num };
}

NColor NColor::operator*(float num) const
{
	return { r * num, g * num, b * num };
}

NColor NColor::operator/(float num) const
{
	return { r / num, g / num, b / num };
}

NColor NColor::operator=(const NColor& col)
{
	r = col.r, g = col.g, b = col.b, a = col.a;
	return *this;
}
