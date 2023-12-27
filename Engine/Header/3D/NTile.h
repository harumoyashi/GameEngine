#pragma once
#include "NObj3d.h"

// タイルオブジェクト //
// 今回のゲーム用に地割れ要素も入ってるので汎用的ではない //

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
	std::array<NVec3,maxObj> objPos_;	//フィールド上にいるオブジェクトの座標

public:
	//初期化
	bool Init()override;
	//更新
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
	//フィールド上にいるオブジェクトの座標設定
	void SetObjPos(NVec3 objPos,uint32_t eleNum);
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