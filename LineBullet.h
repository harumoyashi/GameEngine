#pragma once
#include "IBullet.h"
#include "NTimer.h"

//�ʏ�̒e
//�����Ŗ{����������
class LineBullet final :
	public IBullet
{
private:
	NTimer shotCoolTimer_;		//�e���܂ł̎���
	bool isCanShot_;			//���Ă邩�t���O
	uint32_t level_;            //�e�̋������x��

public:
	LineBullet();

	//�ʏ�e��p�A�b�v�f�[�g
	void LineUpdate();

	//�e���܂ł̎��Ԃ����Z�b�g
	inline void ReSetShotCoolTimer() { shotCoolTimer_.Reset(); }

	// �Q�b�^�[ //
	//���Ă邩�t���O�擾
	inline bool GetIsCanShot() const { return isCanShot_; }
	//�e�̋������x���擾
	inline uint32_t GetLevel() const { return level_; }

	// �Z�b�^�[ //
	//���Ă邩�t���O�ݒ�
	inline void SetIsCanShot(bool isCanShot) { isCanShot_ = isCanShot; }
	//�e�̋������x���ݒ�
	inline void SetLevel(bool level) { level_ = level; }
};

