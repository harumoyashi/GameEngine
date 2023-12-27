#pragma once
#include "NObj3d.h"

// 背景オブジェクト(うねうねしてるやつ) //

class BackObj final :
	public NObj3d
{
private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataBackObj>> cbBackObj_;

	bool isAvoid_;			//避けるようになるか否か
	NVec3 extrusionTimer_;	//押し出しタイマー

public:
	bool Init()override;
	void Update()override;
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//描画
	void Draw()override;

	//避けるようになるか否か設定
	void SetIsAvoid(bool isAvoid) { isAvoid_ = isAvoid; }
	//押し出しタイマー設定
	void SetExtrusionTimer(NVec3 extrusionTimer) { extrusionTimer_ = extrusionTimer; }

private:
	//背景オブジェクト用定数バッファ転送
	void TransferCBBackObj();
};