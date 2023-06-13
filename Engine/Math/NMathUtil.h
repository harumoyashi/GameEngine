#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"
#include "NQuaternion.h"
#include <stdint.h>

const float PI = 3.141592654f;
const float PI2 = PI * 2.0f;

namespace MathUtil
{
	//ビュー変換行列生成
	NMatrix4 MatView(const NVector3& eye, const NVector3& target, const NVector3& up);
	//平行投影変換行列生成
	NMatrix4 ParallelProjection(const float WIN_WIDTH, const float WIN_HEIGHT);
	//透視投影変換行列生成
	NMatrix4 PerspectiveProjection(const float fov, const float aspect, const  float nearZ, const float farZ);

	//度数法→ラジアン
	float Degree2Radian(const float degree);
	//度数法→ラジアン
	float Radian2Degree(const float radian);
	//符号返す
	uint32_t Sign(const uint32_t value);
	//ランダムな値を返す
	uint32_t Random(const uint32_t min, const uint32_t max);
	//最小、最大値設定
	template <typename Type>
	Type Clamp(const Type value, const Type min, const Type max)
	{
		// 値が最大値を上回っていたら最大値を返す
		if (value >= max) return max;

		// 値が最小値を下回っていたら最小値を返す
		if (value <= min) return min;

		// どちらにも当てはまらなかったら値をそのまま返す
		return static_cast<Type>(value);
	}
}