#pragma once
#include "NObj3d.h"

class BackObj final :
	public NObj3d
{
private:
	//定数バッファ//
	std::unique_ptr<NConstBuff<ConstBuffDataBackObj>> cbBackObj_;

	bool isAvoid_;	//避けるようになるか否か

public:
	bool Init()override;
	void Update()override;
	//共通グラフィックスコマンド
	static void CommonBeginDraw();
	//描画
	void Draw()override;

	//避けるようになるか否か設定
	void SetIsAvoid(bool isAvoid) { isAvoid_ = isAvoid; }

private:
	//背景オブジェクト用定数バッファ転送
	void TransferCBBackObj();
};