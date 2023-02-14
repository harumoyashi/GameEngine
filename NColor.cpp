#include "NColor.h"

const NColor NColor::red(255, 0, 0);
const NColor NColor::green(0, 255, 0);
const NColor NColor::blue(0, 0, 255);
const NColor NColor::yellow(255, 255, 0);
const NColor NColor::pink(255, 0, 255);
const NColor NColor::lightblue(0, 255, 255);
const NColor NColor::white(255, 255, 255);
const NColor NColor::black(0, 0, 0);

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
