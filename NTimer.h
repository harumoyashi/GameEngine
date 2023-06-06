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
	float timer;		//タイマー
	float maxTimer;	//タイマーの最大値
	bool isTimeOut;	//タイマーが最大値に達したかフラグ

public:
	NTimer() : timer(0), maxTimer(0), isTimeOut(false){}
	NTimer(const float& maxTimer) : timer(0), maxTimer(maxTimer), isTimeOut(false) {}

	//タイマーリセット
	void Reset();
	//更新(タイマー足されてく)
	void Update();
	//指定した値分タイマーを減らしてく
	void SubTimer(const float& subTimer);

	//セッター
	inline void SetTimer(const float& timer) { this->timer = timer; }
	inline void SetMaxTimer(const float& maxTimer) { this->maxTimer = maxTimer; }
	inline void SetisTimeOut(const bool& isTimeOut) { this->isTimeOut = isTimeOut; }

	//ゲッター
	inline bool GetisTimeOut() { return isTimeOut; }
	inline float GetMaxTimer() { return (float)maxTimer; }
	inline float GetTimer() { return (float)timer; }
	//タイマーの進み具合を取得
	inline float GetTimeRate() { return (float)timer / (float)maxTimer; }
	//システムが開始されてからの時間を取得
	float GetNowTime(const TimeType& timeType);
};