#pragma once
#include "NVec3.h"

// 自作クォータニオン //

struct NMatrix4;

struct NQuaternion {
	float x;
	float y;
	float z;
	float w;

	//単位クォータニオンでの生成
	NQuaternion()
		: x(0.f), y(0.f), z(0.f), w(1.f) {}

	//指定した値での生成
	//下手に値いじりたくない
	NQuaternion(const float x, const float y, const float z, const float w)
		: x(x), y(y), z(z), w(w) {}

	//加算
	NQuaternion operator+(const NQuaternion& q) const;
	//減算
	NQuaternion operator-(const NQuaternion& q) const;
	//乗算(回転の合成)
	NQuaternion operator*(const NQuaternion& q) const;
	//値変更するタイプの加算
	NQuaternion operator+=(const NQuaternion& q);
	//値変更するタイプの減算
	NQuaternion operator-=(const NQuaternion& q);
	//値変更するタイプの乗算(回転の合成)
	NQuaternion& operator*=(const NQuaternion& q);
	//全ての値反転(共役や逆クォータニオンとは違う)
	NQuaternion operator-() const;
	//スカラー倍
	NQuaternion operator*(const float s) const;
	//スカラーで割る
	NQuaternion operator/(const float s) const;
	//値変更するタイプのスカラー倍
	NQuaternion& operator*=(const float s);
	//値変更するタイプのスカラーで割る
	NQuaternion& operator/=(const float s);

	//かけ合わせるクォータニオンを二つとも引数に入れての掛け算
	NQuaternion Mul(const NQuaternion& lhs, const NQuaternion& rhs);
	//単位クォータニオンを返す
	NQuaternion Identity();
	//共役クォータニオンを返す
	NQuaternion Conjugate(const NQuaternion& q);
	//normを取得する
	float GetNorm() const;
	//normを返す
	float Norm(const NQuaternion& q);
	//内積
	static float Dot(const NQuaternion& q0, const NQuaternion& q1);
	//外積
	NVec3 Cross(const NQuaternion& q0, const NQuaternion& q1);
	//正規化したのを返す
	NQuaternion Normalize();
	NQuaternion Normalize(const NQuaternion& q);
	//逆クォータニオンを返す
	NQuaternion Inverse(const NQuaternion& q);
	//線形補間
	static NQuaternion Lerp(const NQuaternion q1, const NQuaternion q2, float t);

	/// <summary>
	/// 任意の回転軸で指定した角度だけ回転した姿勢クォータニオンを生成する
	/// </summary>
	/// <param name="axis">回転軸</param>
	/// <param name="angle">角度(Radian)</param>
	/// <returns>姿勢クォータニオン</returns>
	static NQuaternion MakeAxisAngle(const NVec3& axis, const float angle);

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="vec">回転軸</param>
	/// <param name="angle">角度(Radian)</param>
	/// <returns>姿勢クォータニオン</returns>
	static void SetRota(const NVec3& vec, const float angle);

	/// <summary>
	/// オイラー角で表された回転をクォータニオンで表す
	/// </summary>
	/// <param name="rot">オイラー角</param>
	/// <returns>回転クォータニオン</returns>
	NQuaternion EulerToQuaternion(const NVec3& rot) const;

	/// <summary>
	/// クォータニオンを回転行列に変換
	/// </summary>
	/// <returns>回転行列</returns>
	/// 参考:https://qiita.com/aa_debdeb/items/3d02e28fb9ebfa357eaf
	NMatrix4 QuaternionToMatrix() const;

	/// <summary>
	/// ベクトルをクォータニオンで回転させた結果のベクトルを求める
	/// </summary>
	/// <returns>ベクトル</returns>
	NVec3 RotateVector(const NVec3& vector, const NQuaternion& quaternion) const;

	/// <summary>
	/// 球面線形補間
	/// </summary>
	/// <returns>クォータニオン</returns>
	NQuaternion Slarp(const NQuaternion& q0, const NQuaternion& q1, const float t);

	/// <summary>
	/// ベクトル1からベクトル2への回転を生成
	/// </summary>
	/// <returns>クォータニオン</returns>
	NQuaternion VecToVecRota(const NVec3& u, const NVec3& v);
};