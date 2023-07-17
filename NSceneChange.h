#pragma once
#include "NSprite.h"
#include "NEasing.h"

class NSceneChange
{
private:
	std::unique_ptr<NSprite> sprite_;

	bool isSceneChange_;	//�V�[���؂�ւ��邩�t���O
	bool isSceneChangeNow_;	//�V�[���J�ڒ����t���O

	NEasing::EaseTimer inTimer_ = 0.5f;		//�����Ă���Ƃ��̃^�C�}�[
	NEasing::EaseTimer outTimer_ = 0.5f;	//�o�Ă��Ƃ��̃^�C�}�[

public:
	NSceneChange();
	static NSceneChange* GetInstance();

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��

	void Start();	//�V�[���J�ڊJ�n�I

	// �Q�b�^�[ //
	//�V�[���؂�ւ��邩�t���O�擾
	inline bool GetIsChange()const { return isSceneChange_; }
	//�V�[���J�ڒ����t���O�擾
	inline bool GetIsChangeNow()const { return isSceneChangeNow_; }

	// �Z�b�^�[ //
	//�V�[���؂�ւ��邩�t���O�ݒ�
	inline void SetIsChange(bool isChange) { this->isSceneChange_ = isChange; }
};