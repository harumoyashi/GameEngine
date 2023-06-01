#pragma once
class NColor
{
public:
	float r, g, b, a;

	NColor() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	NColor(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}
	NColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	//色情報変更(int型0~255)
	void SetColor(int R = 255, int G = 255, int B = 255, int A = 255);
	void SetColor(float R = 1.0f, float G = 1.0f, float B = 1.0f, float A = 1.0f);
	//0.0f~1.0fに直す
	void Convert01();
	//0.0f~255.0fに直す
	void Convert255();

	const static NColor red;
	const static NColor green;
	const static NColor blue;
	const static NColor yellow;
	const static NColor pink;
	const static NColor lightblue;
	const static NColor white;
	const static NColor black;

	NColor& operator++();

	//単項演算子
	NColor operator+(float num) const;
	NColor operator-(float num) const;
	NColor operator*(float num) const;
	NColor operator/(float num) const;
	NColor operator=(const NColor& col);
};