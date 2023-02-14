#pragma once
class NColor
{
public:
	float r, g, b, a;

	NColor() : r(255), g(255), b(255), a(255) {}
	NColor(float r, float g, float b) : r(r), g(g), b(b), a(255) {}
	NColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	const static NColor red;
	const static NColor green;
	const static NColor blue;
	const static NColor yellow;
	const static NColor pink;
	const static NColor lightblue;
	const static NColor white;
	const static NColor black;

	NColor& operator++();

	//íPçÄââéZéq
	NColor operator+(float num) const;
	NColor operator-(float num) const;
	NColor operator*(float num) const;
	NColor operator/(float num) const;
};