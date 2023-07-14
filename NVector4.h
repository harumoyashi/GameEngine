#pragma once
struct NVector4
{
	float x;
	float y;
	float z;
	float w;

	NVector4();								//ゼロベクトルとする
	NVector4(const float x, const float y, const float z, const float w);	//x成分,y成分,z成分,w成分を指定しての生成

	//メンバ変数
	float Length() const;						//ノルム(長さ)を求める
	NVector4 Normalize() const;					//正規化をする
	float Dot(const NVector4& v) const;			//内積を求める

	//単項演算子オーバーロード
	NVector4 operator+() const;
	NVector4 operator-() const;
	NVector4 operator*(const NVector4& v);
	NVector4 operator/(const NVector4& v);

	//代入演算子オーバーロード
	NVector4& operator+=(const NVector4& v);
	NVector4& operator-=(const NVector4& v);
	NVector4& operator*=(const NVector4& v);
	NVector4& operator/=(const NVector4& v);
	NVector4& operator*=(const float s);
	NVector4& operator/=(const float s);

	NVector4& operator=(const float num);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const NVector4 operator+(const NVector4& v1, const NVector4& v2);
const NVector4 operator-(const NVector4& v1, const NVector4& v2);
const NVector4 operator*(const NVector4& v, const float s);
const NVector4 operator*(const float s, const NVector4& v);
const NVector4 operator/(const NVector4& v, const float s);