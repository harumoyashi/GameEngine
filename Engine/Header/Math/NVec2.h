#pragma once

// 自作Vector2 //

struct NVec2
{
    // メンバ変数
    float x; // x成分
    float y; // y成分

    // コンストラクタ
    // 零ベクトルとして生成
    NVec2();
    // x,y成分を指定しての生成
    NVec2(const float x, const float y);

    // メンバ関数
    // ノルム(長さ)を求める
    float Length() const;
    // 正規化
    NVec2& Normalize();
    // 内積を求める
    float Dot(const NVec2& v) const;
    // 外積を求める
    float Cross(const NVec2& v) const;

    // 単項演算子オーバーロード
    NVec2 operator+() const;
    NVec2 operator-() const;

    // 代入演算子オーバーロード
    NVec2& operator+=(const NVec2& v);
    NVec2& operator-=(const NVec2& v);
    NVec2& operator*=(const float s);
    NVec2& operator/=(const float s);
};

// 2項演算子オーバーロード
// ※いろんな引数のパターンに対応するため、以下のように準備している
const NVec2 operator+(const NVec2& v1, const NVec2& v2);
const NVec2 operator-(const NVec2& v1, const NVec2& v2);
const NVec2 operator*(const NVec2& v, const float s);
const NVec2 operator*(const float s, const NVec2& v);
const NVec2 operator/(const NVec2& v, const float s);