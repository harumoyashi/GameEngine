#pragma once

struct NVector2
{
    // メンバ変数
    float x; // x成分
    float y; // y成分

    // コンストラクタ
    NVector2();                            // 零ベクトルとして生成
    NVector2(const float x, const float y);            // x,y成分を指定しての生成

    // メンバ関数
    float Length() const;                // ノルム(長さ)を求める
    NVector2& Normalize();                // 正規化
    float Dot(const NVector2& v) const;    // 内積を求める
    float Cross(const NVector2& v) const;// 外積を求める

    // 単項演算子オーバーロード
    NVector2 operator+() const;
    NVector2 operator-() const;

    // 代入演算子オーバーロード
    NVector2& operator+=(const NVector2& v);
    NVector2& operator-=(const NVector2& v);
    NVector2& operator*=(const float s);
    NVector2& operator/=(const float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
const NVector2 operator+(const NVector2& v1, const NVector2& v2);
const NVector2 operator-(const NVector2& v1, const NVector2& v2);
const NVector2 operator*(const NVector2& v, const float s);
const NVector2 operator*(const float s, const NVector2& v);
const NVector2 operator/(const NVector2& v, const float s);