#pragma once
#include "IBullet.h"

//�΂�45�x�ō��E������2������
//�����ɂ��n��A���Ō���
class WideBullet final :
    public IBullet
{
private:
	NEasing::EaseTimer shotCoolTimer_;		//�e���܂ł̎���
	NEasing::EaseTimer shortShotCoolTimer_;	//1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
	bool isCanShot_;			//���Ă邩�t���O
	bool isShortCanShot_;		//���Ă邩�t���O�Z����
	uint32_t level_;            //�e�̋������x��
	uint32_t allAtOnceNum_;     //��x�Ɍ��e�̐�

public:
	WideBullet();

	//�T�C�h�e��p�A�b�v�f�[�g
	void WideUpdate();

	//�e���܂ł̎��Ԃ����Z�b�g
	void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }
	//�e���܂�(�Z���ق�)�̎��Ԃ����Z�b�g
	void ReSetShortShotCoolTimer() { shortShotCoolTimer_.Reset(); }

	// �Q�b�^�[ //
	//���Ă邩�t���O�擾
	bool GetIsCanShot() const { return isCanShot_; }
	//���Ă邩�t���O�Z���Ŏ擾
	bool GetIsShortCanShot() const { return isShortCanShot_; }
	//�e�̋������x���擾
	uint32_t GetLevel() const { return level_; }
	//��x�Ɍ��e�̐��擾	
	uint32_t GetAllAtOnceNum() const { return allAtOnceNum_; }
	float GetShotCoolTimer() const { return shotCoolTimer_.nowTime_; }

	// �Z�b�^�[ //
	//���Ă邩�t���O�ݒ�
	void SetIsCanShot(bool isCanShot) { isCanShot_ = isCanShot; }
	//���Ă邩�t���O�ݒ�
	void SetIsShortCanShot(bool isShortCanShot) { isShortCanShot_ = isShortCanShot; }
	//�e�̋������x���ݒ�
	void SetLevel(const uint32_t level) { level_ = level; }
	//��x�Ɍ��e�̐��ݒ�
	void SetAllAtOnceNum(const uint32_t allAtOnceNum) { allAtOnceNum_ = allAtOnceNum; }
};