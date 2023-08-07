#pragma once

//時間取得の際に使う時間の種類
enum class TimeType
{
	MilliSecond,
	Second,
	Minute
};

class NTimer final
{
private:
	float timer_;		//タイマー
	float maxTimer_;	//タイマーの最大値
	bool isTimeOut_;	//タイマーが最大値に達したかフラグ

public:
	NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false){}
	NTimer(const float maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

	//タイマーリセット
	void Reset();
	//更新(タイマー足されてく)
	//elapseTimer:経過時間。タイマー加算量に影響する。スローモーション用
	void Update(const bool isRoop,const float elapseTimer = 1.0f);
	//指定した値分タイマーを減らしてく
	void SubTimer(const float subTimer);

	//セッター
	void SetTimer(const float timer) { timer_ = timer; }
	void SetMaxTimer(const float maxTime_r) { maxTimer_ = maxTime_r; }
	void SetisTimeOut(bool isTimeOut) { isTimeOut_ = isTimeOut; }

	//ゲッター
	bool GetisTimeOut()const { return isTimeOut_; }
	const float GetMaxTimer()const { return (float)maxTimer_; }
	const float GetTimer()const { return (float)timer_; }
	//タイマーの進み具合を取得
	const float GetTimeRate()const { return (float)timer_ / (float)maxTimer_; }
	//システムが開始されてからの時間を取得
	float GetNowTime(const TimeType& timeType);
};