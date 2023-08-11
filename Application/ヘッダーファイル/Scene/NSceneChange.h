#pragma once
#include "NSprite.h"
#include "NEasing.h"

class NSceneChange
{
private:
	std::unique_ptr<NSprite> titleLogo_;	//�^�C�g�����S
	std::unique_ptr<NSprite> blackBack_;	//�w�i�̍����z

	bool isSceneChange_;	//�V�[���؂�ւ��邩�t���O
	bool isSceneChangeNow_;	//�V�[���J�ڒ����t���O

	NEasing::EaseTimer inTimer_ = 0.5f;		//�����Ă���Ƃ��̃^�C�}�[
	NEasing::EaseTimer outTimer_ = 0.5f;	//�o�Ă��Ƃ��̃^�C�}�[

	NVec2 pos_;	//�Ö��̍��W

public:
	NSceneChange();
	static NSceneChange* GetInstance();

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��

	void Start();	//�V�[���J�ڊJ�n�I

	// �Q�b�^�[ //
	//�V�[���؂�ւ��邩�t���O�擾
	bool GetIsChange()const { return isSceneChange_; }
	//�V�[���J�ڒ����t���O�擾
	bool GetIsChangeNow()const { return isSceneChangeNow_; }

	// �Z�b�^�[ //
	//�V�[���؂�ւ��邩�t���O�ݒ�
	void SetIsChange(bool isChange) { this->isSceneChange_ = isChange; }

private:
	NSceneChange(const NSceneChange&) = delete;
	~NSceneChange() = default;
	NSceneChange& operator=(const NSceneChange&) = delete;
};