#pragma once

// 自作Vector4 //

struct NVec4
{
	float x;
	float y;
	float z;
	float w;

	//ゼロベクトルとする
	NVec4();
	//x成分,y成分,z成分,w成分を指定しての生成
	NVec4(const float x, const float y, const float z, const float w);

	//メンバ変数
	//ノルム(長さ)を求める
	float Length() const;
	//正規化をする
	NVec4 Normalize() const;
	//内積を求める
	float Dot(const NVec4& v) const;

	//単項演算子オーバーロード
	//加算
	NVec4 operator+() const;
	//減算
	NVec4 operator-() const;
	//乗算
	NVec4 operator*(const NVec4& v);
	//除算
	NVec4 operator/(const NVec4& v);

	//代入演算子オーバーロード
	//加算
	NVec4& operator+=(const NVec4& v);
	//減算
	NVec4& operator-=(const NVec4& v);
	//乗算
	NVec4& operator*=(const NVec4& v);
	//除算
	NVec4& operator/=(const NVec4& v);
	//floatとの乗算
	NVec4& operator*=(const float s);
	//floatとの除算
	NVec4& operator/=(const float s);
	//代入
	NVec4& operator=(const float num);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
//加算
const NVec4 operator+(const NVec4& v1, const NVec4& v2);
//減算
const NVec4 operator-(const NVec4& v1, const NVec4& v2);
//floatとの乗算
const NVec4 operator*(const NVec4& v, const float s);
//floatとの乗算
const NVec4 operator*(const float s, const NVec4& v);
//floatとの除算
const NVec4 operator/(const NVec4& v, const float s);