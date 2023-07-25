#pragma once
#include "NObj3d.h"

class NTile final :
	public NObj3d
{
private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataTile>> cbTile_;

	float divide_;			//タイルの分割幅
	float activityArea_;	//行動範囲

public:
	bool Init()override;
	void Update()override;
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//描画
	void Draw()override;

	// セッター //
	//タイルの分割幅設定
	void SetDivide(float divide) { divide_ = divide; }
	//行動範囲設定
	void SetActivityArea(float activityArea) { activityArea_ = activityArea; }

private:
	//タイル用定数バッファ転送
	void TransferCBTile();
};