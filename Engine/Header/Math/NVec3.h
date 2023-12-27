#pragma once

// 自作Vector3 //

struct NVec3
{
	float x;
	float y;
	float z;

	NVec3();								//ゼロベクトルとする
	NVec3(const float x, const float y, const float z);	//x成分,y成分,z成分を指定しての生成

	//代入楽する用
	const static NVec3 zero;
	const static NVec3 one;
	const static NVec3 up;
	const static NVec3 front;
	const static NVec3 right;

	//メンバ関数
	float Length() const;						//ノルム(長さ)を求める
	NVec3 Normalize() const;					//正規化をする
	float Dot(const NVec3& v) const;			//内積を求める
	NVec3 Cross(const NVec3& v) const;			//外積を求める
	//線形補間
	static NVec3 Lerp(const NVec3 start, const NVec3 end, const float timer);

	//単項演算子オーバーロード
	NVec3 operator+() const;
	NVec3 operator-() const;
	NVec3 operator*(const NVec3& v);
	NVec3 operator/(const NVec3& v);

	//代入演算子オーバーロード
	NVec3& operator+=(const NVec3& v);
	NVec3& operator-=(const NVec3& v);
	NVec3& operator*=(const NVec3& v);
	NVec3& operator/=(const NVec3& v);
	NVec3& operator*=(const float s);
	NVec3& operator/=(const float s);

	NVec3& operator=(float num);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
const NVec3 operator+(const NVec3& v1, const NVec3& v2);
const NVec3 operator-(const NVec3& v1, const NVec3& v2);
const NVec3 operator*(const NVec3& v, const float s);
const NVec3 operator*(const float s, const NVec3& v);
const NVec3 operator/(const NVec3& v, const float s);