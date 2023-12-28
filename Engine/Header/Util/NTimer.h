#pragma once

// タイマー //

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
	//コンストラクタ
	NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false) {}
	//タイマーの最大値を指定指定して生成
	NTimer(const float maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

	//タイマーリセット
	void Reset();
	//更新(タイマー足されてく)
	//elapseTimer:経過時間。タイマー加算量に影響する。スローモーション用
	void Update(const bool isRoop, const float elapseTimer = 1.0f);
	//指定した値分タイマーを減らしてく
	void SubTimer(const float subTimer, const float elapseTimer = 1.0f);

	// ゲッター //
	//タイマー取得
	const float GetTimer()const { return (float)timer_; }
	//タイマーの最大値取得
	const float GetMaxTimer()const { return (float)maxTimer_; }
	//タイマーが最大値に達したかフラグ取得
	bool GetisTimeOut()const { return isTimeOut_; }
	//タイマーの進み具合を取得
	const float GetTimeRate()const { return (float)timer_ / (float)maxTimer_; }
	//システムが開始されてからの時間を取得
	float GetNowTime(const TimeType& timeType);

	// セッター //
	//タイマー設定
	void SetTimer(const float timer) { timer_ = timer; }
	//タイマーの最大値設定
	void SetMaxTimer(const float maxTime_r) { maxTimer_ = maxTime_r; }
	//タイマーが最大値に達したかフラグ設定
	void SetisTimeOut(bool isTimeOut) { isTimeOut_ = isTimeOut; }
};