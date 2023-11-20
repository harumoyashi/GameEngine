#pragma once
#include "NObj3d.h"

class NTile final :
	public NObj3d
{
private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTile>> cbTile_;

	float divide_;						//タイルの分割幅
	float activityArea_;				//行動範囲
	bool isAvoid_;						//避けるようになるか否か
	float avoidArea_;					//避ける範囲
	float floatingTimer_;				//ふよふよタイマー
	float wavePosZ_;					//波のZ座標

public:
	bool Init()override;
	void Update()override;
	//共通グラフィックスコマンド
	static void CommonBeginDraw(bool isAvoid);
	//描画
	void Draw()override;

	// セッター //
	//タイルの分割幅設定
	void SetDivide(float divide) { divide_ = divide; }
	//行動範囲設定
	void SetActivityArea(float activityArea) { activityArea_ = activityArea; }
	//波のZ座標設定
	void SetWavePosZ(float wavePosZ) { wavePosZ_ = wavePosZ; }
	//避けるようになるか否か設定
	void SetIsAvoid(bool isAvoid) { isAvoid_ = isAvoid; }
	//避ける範囲設定
	void SetAvoidArea(float avoidArea) { avoidArea_ = avoidArea; }
	//ふよふよタイマー設定
	void SetFloatingTimer(float floatingTimer) { floatingTimer_ = floatingTimer; }

private:
	//タイル用定数バッファ転送
	void TransferCBTile();
};