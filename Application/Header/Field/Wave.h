#pragma once
#include "NObj3d.h"
#include "NSprite.h"
#include "NEasing.h"
#include "NumDrower.h"

// 波 //

class Wave final
{
private:
	std::vector<std::unique_ptr<NObj3d>> obj_;	//オブジェクト群
	std::vector<float> randomPosZ_;				//各波のうにょうにょ
	const uint32_t waveDivide_ = 200;			//波の分割数

	float scaleZ_;								//波の大きさ(zだけ)
	float posZ_;								//波全体の共通座標(z座標)
	float moveSpeed_;							//移動速度
	bool isMove_;								//移動していいか

	std::vector<NEasing::EaseTimer> waveTimer_;	//波のうねり用タイマー

	std::unique_ptr<NSprite> waveUI_;			//波が迫ってるか知らせるUI
	std::unique_ptr<NSprite> meterUI_;			//波がどのくらい迫ってるか知らせるUI
	uint32_t player2WaveLen_;					//プレイヤーと波の距離
	NumDrower meterTex_;						//プレイヤーと波の距離表示

public:
	//シングルトンインスタンス取得
	static Wave* GetInstance();

	//初期化
	void Init();
	//更新
	void Update();
	//オブジェクト描画
	void DrawObj();
	//スプライト描画
	void DrawSprite();

	// ゲッター //
	//波の前方座標取得
	float GetFrontPosZ() const { return posZ_ + scaleZ_; }

	// セッター //
	//波が動くかフラグ設定
	void SetIsMove(bool isMove) { isMove_ = isMove; }

private:
	//コンストラクタ
	Wave() = default;
	//コピー禁止
	Wave(const Wave&) = delete;
	//デストラクタ
	~Wave() = default;
	//コピー禁止
	Wave& operator=(const Wave&) = delete;
};