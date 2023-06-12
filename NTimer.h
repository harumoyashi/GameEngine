#pragma once

//���Ԏ擾�̍ۂɎg�����Ԃ̎��
enum class TimeType
{
	MilliSecond,
	Second,
	Minute
};

class NTimer
{
private:
	float timer_;		//�^�C�}�[
	float maxTimer_;	//�^�C�}�[�̍ő�l
	bool isTimeOut_;	//�^�C�}�[���ő�l�ɒB�������t���O

public:
	NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false){}
	NTimer(const float& maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

	//�^�C�}�[���Z�b�g
	void Reset();
	//�X�V(�^�C�}�[������Ă�)
	void Update();
	//�w�肵���l���^�C�}�[�����炵�Ă�
	void SubTimer(const float& subTimer);

	//�Z�b�^�[
	inline void SetTimer(const float& timer) { timer_ = timer; }
	inline void SetMaxTimer(const float& maxTimer) { maxTimer_ = maxTimer; }
	inline void SetisTimeOut(const bool& isTimeOut) { isTimeOut_ = isTimeOut; }

	//�Q�b�^�[
	inline bool GetisTimeOut() { return isTimeOut_; }
	inline float GetMaxTimer() { return (float)maxTimer_; }
	inline float GetTimer() { return (float)timer_; }
	//�^�C�}�[�̐i�݋���擾
	inline float GetTimeRate() { return (float)timer_ / (float)maxTimer_; }
	//�V�X�e�����J�n����Ă���̎��Ԃ��擾
	float GetNowTime(const TimeType& timeType);
};