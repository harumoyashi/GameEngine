#pragma once

//時間取得の際に使う時間の種類
enum class TimeType
{
	MilliSecond,
	Second,
	Minute
};

class NTimer
{
private:
	float timer_;		//タイマー
	float maxTimer_;	//タイマーの最大値
	bool isTimeOut_;	//タイマーが最大値に達したかフラグ

public:
	NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false){}
	NTimer(const float& maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

	//タイマーリセット
	void Reset();
	//更新(タイマー足されてく)
	void Update();
	//指定した値分タイマーを減らしてく
	void SubTimer(const float& subTimer);

	//セッター
	inline void SetTimer(const float& timer) { timer_ = timer; }
	inline void SetMaxTimer(const float& maxTimer) { maxTimer_ = maxTimer; }
	inline void SetisTimeOut(const bool& isTimeOut) { isTimeOut_ = isTimeOut; }

	//ゲッター
	inline bool GetisTimeOut() { return isTimeOut_; }
	inline float GetMaxTimer() { return (float)maxTimer_; }
	inline float GetTimer() { return (float)timer_; }
	//タイマーの進み具合を取得
	inline float GetTimeRate() { return (float)timer_ / (float)maxTimer_; }
	//システムが開始されてからの時間を取得
	float GetNowTime(const TimeType& timeType);
};