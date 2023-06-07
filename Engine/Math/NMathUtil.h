#pragma once
#include "NVector2.h"
#include "NVector3.h"
#include "NMatrix4.h"
#include "NQuaternion.h"
#include <stdint.h>

namespace MathUtil
{
	//ビュー変換行列生成
	NMatrix4 MatView(NVector3 eye, NVector3 target, NVector3 up);
	//平行投影変換行列生成
	NMatrix4 ParallelProjection(float WIN_WIDTH, float WIN_HEIGHT);
	//透視投影変換行列生成
	NMatrix4 PerspectiveProjection(float fov, float aspect, float nearZ, float farZ);

	//度数法→ラジアン
	float Degree2Radian(float degree);
	//度数法→ラジアン
	float Radian2Degree(float radian);
	//符号返す
	uint32_t Sign(uint32_t value);
	//ランダムな値を返す
	uint32_t Random(uint32_t min,uint32_t max);
	//最小、最大値設定
	template <typename Type>
	Type Clamp(Type value, Type min, Type max)
	{
		// 値が最大値を上回っていたら最大値を返す
		if (value >= max) return max;

		// 値が最小値を下回っていたら最小値を返す
		if (value <= min) return min;

		// どちらにも当てはまらなかったら値をそのまま返す
		return static_cast<Type>(value);
	}
}