#pragma once
#include "NObj3d.h"
#include "NEasing.h"

class Wave final
{
private:
	std::vector<std::unique_ptr<NObj3d>> obj_;	//オブジェクト群
	const uint32_t waveDivide_ = 1;				//波の分割数

	float scaleZ_;								//波の大きさ(zだけ)
	float posZ_;								//波全体の共通座標(z座標)
	float moveSpeed_;							//移動速度

public:
	static Wave* GetInstance();

	void Init();
	void Update();
	void Draw();

	// ゲッター //
	//波の前方座標取得
	float GetFrontPosZ() const { return posZ_ + scaleZ_; }

	// セッター //

private:
	Wave() = default;
	Wave(const Wave&) = delete;
	~Wave() = default;
	Wave& operator=(const Wave&) = delete;
};