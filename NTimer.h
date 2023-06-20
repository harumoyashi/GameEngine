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
	float maxTime_r_;	//�^�C�}�[�̍ő�l
	bool isTimeOut_;	//�^�C�}�[���ő�l�ɒB�������t���O

public:
	NTimer() : timer_(0), maxTime_r_(0), isTimeOut_(false){}
	NTimer(const float maxTime_r) : timer_(0), maxTime_r_(maxTime_r), isTimeOut_(false) {}

	//�^�C�}�[���Z�b�g
	void Reset();
	//�X�V(�^�C�}�[������Ă�)
	void Update();
	//�w�肵���l���^�C�}�[�����炵�Ă�
	void SubTimer(const float subTimer);

	//�Z�b�^�[
	inline void SetTimer(const float timer) { timer_ = timer; }
	inline void SetMaxTimer(const float maxTime_r) { maxTime_r_ = maxTime_r; }
	inline void SetisTimeOut(const bool isTimeOut) { isTimeOut_ = isTimeOut; }

	//�Q�b�^�[
	inline const bool GetisTimeOut()const { return isTimeOut_; }
	inline const float GetMaxTimer()const { return (float)maxTime_r_; }
	inline const float GetTimer()const { return (float)timer_; }
	//�^�C�}�[�̐i�݋���擾
	inline const float GetTimeRate()const { return (float)timer_ / (float)maxTime_r_; }
	//�V�X�e�����J�n����Ă���̎��Ԃ��擾
	float GetNowTime(const TimeType& timeType);
};