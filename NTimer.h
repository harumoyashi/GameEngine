#pragma once
class NTimer
{
private:
	int timer;		//�^�C�}�[
	int maxTimer;	//�^�C�}�[�̍ő�l
	bool isTimeOut;	//�^�C�}�[���ő�l�ɒB�������t���O

public:
	NTimer() : timer(0), maxTimer(0), isTimeOut(false){}
	NTimer(const int& maxTimer) : timer(0), maxTimer(maxTimer), isTimeOut(false) {}

	//�^�C�}�[���Z�b�g
	void Reset();
	//�X�V(�^�C�}�[������Ă�)
	void Update();
	//�w�肵���l���^�C�}�[�����炵�Ă�
	void SubTimer(const float& subTimer);

	//�Z�b�^�[
	inline void SetTimer(const int& timer) { this->timer = timer; }
	inline void SetMaxTimer(const int& maxTimer) { this->maxTimer = maxTimer; }
	inline void SetisTimeOut(const bool& isTimeOut) { this->isTimeOut = isTimeOut; }

	//�Q�b�^�[
	inline bool GetisTimeOut() { return isTimeOut; }
	inline float GetMaxTimer() { return (float)maxTimer; }
	inline float GetTimer() { return (float)timer; }
	//�^�C�}�[�̐i�݋���擾
	inline float GetTimeRate() { return (float)timer / (float)maxTimer; }
};