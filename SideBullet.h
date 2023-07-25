#pragma once
#include "IBullet.h"

//���E������2������
//�����ɂ��n��A���Ō���
class SideBullet final :
	public IBullet
{
private:
    NEasing::EaseTimer shotCoolTimer_;		//�e���܂ł̎���
    NEasing::EaseTimer shortShotCoolTimer_;	//1���[�v�œ��ȏ�ł��ɒe���܂ł̎���
    bool isCanShot_;			//���Ă邩�t���O
    uint32_t level_;            //�e�̋������x��
    uint32_t allAtOnceNum_;     //��x�Ɍ��e�̐�

public:
    SideBullet();

	//�T�C�h�e��p�A�b�v�f�[�g
    void SideUpdate();

	//�e���܂ł̎��Ԃ����Z�b�g
	void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }

	// �Q�b�^�[ //
	//���Ă邩�t���O�擾
	bool GetIsCanShot() const { return isCanShot_; }
	//�e�̋������x���擾
	uint32_t GetLevel() const { return level_; }
	//��x�Ɍ��e�̐��擾	
	uint32_t GetAllAtOnceNum() const { return allAtOnceNum_; }
	float GetShotCoolTimer() const { return shotCoolTimer_.nowTime_; }

	// �Z�b�^�[ //
	//���Ă邩�t���O�ݒ�
	void SetIsCanShot(bool isCanShot) { isCanShot_ = isCanShot; }
	//�e�̋������x���ݒ�
	void SetLevel(const uint32_t level) { level_ = level; }
	//��x�Ɍ��e�̐��ݒ�
	void SetAllAtOnceNum(const uint32_t allAtOnceNum) { allAtOnceNum_ = allAtOnceNum; }
};