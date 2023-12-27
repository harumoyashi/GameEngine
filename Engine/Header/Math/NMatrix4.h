#pragma once
#include "NQuaternion.h"
#include "NVec3.h"
#include "NVec4.h"

// 自作4*4の行列 //

struct NMatrix4
{
	float m[4][4];

	//単位行列で生成
	NMatrix4() :
		m{
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1 } {}
	NMatrix4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33) :
		m{
			_00, _01, _02, _03,
			_10, _11, _12, _13,
			_20, _21, _22, _23,
			_30, _31, _32, _33 } {}

	//逆行列
	NMatrix4 operator-() const;

	//代入演算子オーバーロード
	//加算
	NMatrix4 operator+(const NMatrix4& mat) const;
	//加算
	NMatrix4& operator+=(const NMatrix4& mat);
	//減算
	NMatrix4 operator-(const NMatrix4& mat) const;
	//減算
	NMatrix4& operator-=(const NMatrix4& mat);
	//乗算
	NMatrix4 operator*(const NMatrix4& mat) const;
	//乗算
	NMatrix4& operator*=(const NMatrix4& mat);

	//全ての要素が0の行列
	static NMatrix4 Zero();
	//単位行列
	static NMatrix4 Identity();
	//拡縮行列の設定
	static NMatrix4 Scale(const NVec3& s);
	//回転行列の設定
	static NMatrix4 RotateX(const float angle);
	static NMatrix4 RotateY(const float angle);
	static NMatrix4 RotateZ(const float angle);
	//クォータニオンから回転行列に変換
	static NMatrix4 RotateQuaternion(const NQuaternion& q);
	//平行移動行列の作成
	static NMatrix4 Translation(const NVec3& t);
	//座標変換（ベクトルと行列の掛け算）
	static NVec3 Transform(const NVec3& v, const NMatrix4& mat);
	//w除算を使った座標変換
	static NVec3 WTransform(const NVec3& v, const NMatrix4& mat);
	NVec4 Vec4MulMat(const NVec4& v, const NMatrix4& mat)const;

	//オイラー角への変換
	//参考:https://qiita.com/aa_debdeb/items/3d02e28fb9ebfa357eaf#%E5%9B%9E%E8%BB%A2%E8%A1%8C%E5%88%97%E3%81%8B%E3%82%89%E3%82%AA%E3%82%A4%E3%83%A9%E3%83%BC%E8%A7%92
	NVec3 ToEuler() const;

	//転置行列
	NMatrix4 Transpose();

	//行列からワールド座標情報を抽出
	NVec3 GetWorldPos() const;
	//行列からスケール情報を抽出
	NVec3 GetScale() const;
};

NVec3 operator*(const NVec3& v, const NMatrix4& mat);
NVec4 operator*(const NVec4& v, const NMatrix4& mat);
NVec3& operator*=(NVec3& v, const NMatrix4& mat);