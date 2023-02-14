#pragma once
class NTimer
{
private:
	int timer;		//タイマー
	int maxTimer;	//タイマーの最大値
	bool isTimeOut;	//タイマーが最大値に達したかフラグ

public:
	NTimer() : timer(0), maxTimer(0), isTimeOut(false){}
	NTimer(const int& maxTimer) : timer(0), maxTimer(maxTimer), isTimeOut(false) {}

	//タイマーリセット
	void Reset();
	//更新(タイマー足されてく)
	void Update();
	//指定した値分タイマーを減らしてく
	void SubTimer(const float& subTimer);

	//セッター
	inline void SetTimer(const int& timer) { this->timer = timer; }
	inline void SetMaxTimer(const int& maxTimer) { this->maxTimer = maxTimer; }
	inline void SetisTimeOut(const bool& isTimeOut) { this->isTimeOut = isTimeOut; }

	//ゲッター
	inline bool GetisTimeOut() { return isTimeOut; }
	inline float GetMaxTimer() { return (float)maxTimer; }
	inline float GetTimer() { return (float)timer; }
	//タイマーの進み具合を取得
	inline float GetTimeRate() { return (float)timer / (float)maxTimer; }
};