#pragma once

struct NVector3
{
	float x;
	float y;
	float z;

	NVector3();								//ゼロベクトルとする
	NVector3(const float x, const float y, const float z);	//x成分,y成分,z成分を指定しての生成

	//メンバ変数
	float Length() const;						//ノルム(長さ)を求める
	NVector3 Normalize() const;					//正規化をする
	float Dot(const NVector3& v) const;			//内積を求める
	NVector3 Cross(const NVector3& v) const;	//外積を求める

	//単項演算子オーバーロード
	NVector3 operator+() const;
	NVector3 operator-() const;
	NVector3 operator*(const NVector3& v);
	NVector3 operator/(const NVector3& v);

	//代入演算子オーバーロード
	NVector3& operator+=(const NVector3& v);
	NVector3& operator-=(const NVector3& v);
	NVector3& operator*=(const NVector3& v);
	NVector3& operator/=(const NVector3& v);
	NVector3& operator*=(const float s);
	NVector3& operator/=(const float s);

	NVector3& operator=(const float num);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const NVector3 operator+(const NVector3& v1, const NVector3& v2);
const NVector3 operator-(const NVector3& v1, const NVector3& v2);
const NVector3 operator*(const NVector3& v, const float s);
const NVector3 operator*(const float s, const NVector3& v);
const NVector3 operator/(const NVector3& v, const float s);