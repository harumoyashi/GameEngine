#pragma once
#include "IBullet.h"
#include "NTimer.h"

//���E������2������
//�����ɂ��n��A���Ō���
class SideBullet:
	public IBullet
{
private:
    NTimer shotCoolTimer_;		//�e���܂ł̎���
    NTimer shortShotCoolTimer_;	//1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    bool isCanShot_;			//���Ă邩�t���O
    uint32_t level_;            //�e�̋������x��
    uint32_t allAtOnceNum_;     //��x�Ɍ��e�̐�

public:
    SideBullet();

	//�T�C�h�e��p�A�b�v�f�[�g
    void SideUpdate();

	//�e���܂ł̎��Ԃ����Z�b�g
	inline void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }

	// �Q�b�^�[ //
	//���Ă邩�t���O�擾
	inline bool GetIsCanShot() const { return isCanShot_; }
	//�e�̋������x���擾
	inline uint32_t GetLevel() const { return level_; }
	//��x�Ɍ��e�̐��擾	
	inline uint32_t GetAllAtOnceNum() const { return allAtOnceNum_; }
	inline float GetShotCoolTimer() const { return shotCoolTimer_.GetTimeRate(); }

	// �Z�b�^�[ //
	//���Ă邩�t���O�ݒ�
	inline void SetIsCanShot(const bool isCanShot) { isCanShot_ = isCanShot; }
	//�e�̋������x���ݒ�
	inline void SetLevel(const uint32_t level) { level_ = level; }
	//��x�Ɍ��e�̐��ݒ�
	inline void SetAllAtOnceNum(const uint32_t allAtOnceNum) { allAtOnceNum_ = allAtOnceNum; }
};