#pragma once
#include "IBullet.h"
#include "NTimer.h"

//左右同時に2発撃つ
//強化によりn回連続で撃つ
class SideBullet:
	public IBullet
{
private:
    NTimer shotCoolTimer_;		//弾撃つまでの時間
    NTimer shortShotCoolTimer_;	//1ループで二回以上打つ時に弾撃つまでの時間
    bool isCanShot_;			//撃てるかフラグ
    uint32_t level_;            //弾の強化レベル
    uint32_t allAtOnceNum_;     //一度に撃つ弾の数

public:
    SideBullet();

	//サイド弾専用アップデート
    void SideUpdate();

	//弾撃つまでの時間をリセット
	inline void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }

	// ゲッター //
	//撃てるかフラグ取得
	inline bool GetIsCanShot() const { return isCanShot_; }
	//弾の強化レベル取得
	inline uint32_t GetLevel() const { return level_; }
	//一度に撃つ弾の数取得	
	inline uint32_t GetAllAtOnceNum() const { return allAtOnceNum_; }
	inline float GetShotCoolTimer() const { return shotCoolTimer_.GetTimeRate(); }

	// セッター //
	//撃てるかフラグ設定
	inline void SetIsCanShot(const bool isCanShot) { isCanShot_ = isCanShot; }
	//弾の強化レベル設定
	inline void SetLevel(const uint32_t level) { level_ = level; }
	//一度に撃つ弾の数設定
	inline void SetAllAtOnceNum(const uint32_t allAtOnceNum) { allAtOnceNum_ = allAtOnceNum; }
};