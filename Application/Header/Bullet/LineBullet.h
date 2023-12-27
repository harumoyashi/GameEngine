#pragma once
#include "IBullet.h"

// 通常の弾 //
// 強化で本数が増える //

class LineBullet final :
	public IBullet
{
private:
	NEasing::EaseTimer shotCoolTimer_;		//弾撃つまでの時間
	bool isCanShot_;						//撃てるかフラグ
	uint32_t level_;						//弾の強化レベル

public:
	//コンストラクタ
	LineBullet();

	//通常弾専用アップデート
	void LineUpdate();

	//弾撃つまでの時間をリセット
	void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }

	// ゲッター //
	//撃てるかフラグ取得
	bool GetIsCanShot() const { return isCanShot_; }
	//弾の強化レベル取得
	uint32_t GetLevel() const { return level_; }

	// セッター //
	//撃てるかフラグ設定
	void SetIsCanShot(bool isCanShot) { isCanShot_ = isCanShot; }
	//弾の強化レベル設定
	void SetLevel(uint32_t level) { level_ = level; }
};

