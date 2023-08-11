#pragma once

//���Ԏ擾�̍ۂɎg�����Ԃ̎��
enum class TimeType
{
	MilliSecond,
	Second,
	Minute
};

class NTimer final
{
private:
	float timer_;		//�^�C�}�[
	float maxTimer_;	//�^�C�}�[�̍ő�l
	bool isTimeOut_;	//�^�C�}�[���ő�l�ɒB�������t���O

public:
	NTimer() : timer_(0), maxTimer_(0), isTimeOut_(false){}
	NTimer(const float maxTimer) : timer_(0), maxTimer_(maxTimer), isTimeOut_(false) {}

	//�^�C�}�[���Z�b�g
	void Reset();
	//�X�V(�^�C�}�[������Ă�)
	//elapseTimer:�o�ߎ��ԁB�^�C�}�[���Z�ʂɉe������B�X���[���[�V�����p
	void Update(const bool isRoop,const float elapseTimer = 1.0f);
	//�w�肵���l���^�C�}�[�����炵�Ă�
	void SubTimer(const float subTimer);

	//�Z�b�^�[
	void SetTimer(const float timer) { timer_ = timer; }
	void SetMaxTimer(const float maxTime_r) { maxTimer_ = maxTime_r; }
	void SetisTimeOut(bool isTimeOut) { isTimeOut_ = isTimeOut; }

	//�Q�b�^�[
	bool GetisTimeOut()const { return isTimeOut_; }
	const float GetMaxTimer()const { return (float)maxTimer_; }
	const float GetTimer()const { return (float)timer_; }
	//�^�C�}�[�̐i�݋���擾
	const float GetTimeRate()const { return (float)timer_ / (float)maxTimer_; }
	//�V�X�e�����J�n����Ă���̎��Ԃ��擾
	float GetNowTime(const TimeType& timeType);
};