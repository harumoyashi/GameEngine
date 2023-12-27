#pragma once
#include "IBullet.h"

// 左右同時に2発撃つ //
// 強化によりn回連続で撃つ //

class SideBullet final :
	public IBullet
{
private:
	NEasing::EaseTimer shotCoolTimer_;		//弾撃つまでの時間
	NEasing::EaseTimer shortShotCoolTimer_;	//1ループで二回以上打つ時に弾撃つまでの時間
	bool isCanShot_;			//撃てるかフラグ
	bool isShortCanShot_;		//撃てるかフラグ短い版
	uint32_t level_;            //弾の強化レベル
	uint32_t allAtOnceNum_;     //一度に撃つ弾の数

public:
	SideBullet();

	//サイド弾専用アップデート
	void SideUpdate();

	//弾撃つまでの時間をリセット
	void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }
	//弾撃つまで(短いほう)の時間をリセット
	void ReSetShortShotCoolTimer() { shortShotCoolTimer_.Reset(); }

	// ゲッター //
	//撃てるかフラグ取得
	bool GetIsCanShot() const { return isCanShot_; }
	//撃てるかフラグ短い版取得
	bool GetIsShortCanShot() const { return isShortCanShot_; }
	//弾の強化レベル取得
	uint32_t GetLevel() const { return level_; }
	//一度に撃つ弾の数取得	
	uint32_t GetAllAtOnceNum() const { return allAtOnceNum_; }
	float GetShotCoolTimer() const { return shotCoolTimer_.nowTime_; }

	// セッター //
	//撃てるかフラグ設定
	void SetIsCanShot(bool isCanShot) { isCanShot_ = isCanShot; }
	//撃てるかフラグ設定
	void SetIsShortCanShot(bool isShortCanShot) { isShortCanShot_ = isShortCanShot; }
	//弾の強化レベル設定
	void SetLevel(const uint32_t level) { level_ = level; }
	//一度に撃つ弾の数設定
	void SetAllAtOnceNum(const uint32_t allAtOnceNum) { allAtOnceNum_ = allAtOnceNum; }
};