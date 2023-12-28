#pragma once

// 自作Vector3 //

struct NVec3
{
	float x;
	float y;
	float z;

	//ゼロベクトルとする
	NVec3();
	//x成分,y成分,z成分を指定しての生成
	NVec3(const float x, const float y, const float z);

	//代入楽する用
	const static NVec3 zero;
	const static NVec3 one;
	const static NVec3 up;
	const static NVec3 front;
	const static NVec3 right;

	//メンバ関数
	//ノルム(長さ)を求める
	float Length() const;
	//正規化をする
	NVec3 Normalize() const;
	//内積を求める
	float Dot(const NVec3& v) const;
	//外積を求める
	NVec3 Cross(const NVec3& v) const;
	//線形補間
	static NVec3 Lerp(const NVec3 start, const NVec3 end, const float timer);

	//単項演算子オーバーロード
	//加算
	NVec3 operator+() const;
	//減算
	NVec3 operator-() const;
	//乗算
	NVec3 operator*(const NVec3& v);
	//除算
	NVec3 operator/(const NVec3& v);

	//代入演算子オーバーロード
	//加算
	NVec3& operator+=(const NVec3& v);
	//減算
	NVec3& operator-=(const NVec3& v);
	//乗算
	NVec3& operator*=(const NVec3& v);
	//除算
	NVec3& operator/=(const NVec3& v);
	//floatとの乗算
	NVec3& operator*=(const float s);
	//floatとの除算
	NVec3& operator/=(const float s);
	//代入
	NVec3& operator=(float num);
};

//2項演算子オーバーロード
//※いろんな引数のパターンに対応(引数の順序)するため、以下のように準備している
//加算
const NVec3 operator+(const NVec3& v1, const NVec3& v2);
//減算
const NVec3 operator-(const NVec3& v1, const NVec3& v2);
//floatとの乗算
const NVec3 operator*(const NVec3& v, const float s);
//floatとの乗算
const NVec3 operator*(const float s, const NVec3& v);
//floatとの除算
const NVec3 operator/(const NVec3& v, const float s);