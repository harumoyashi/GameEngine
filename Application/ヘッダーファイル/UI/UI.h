#pragma once
#include <memory>
#include <vector>
#include "NSprite.h"
#include "NEasing.h"

enum class UIType
{
	Abutton,	//A�{�^��
	AbuttonPush,//A�{�^�������ꂽ���
	Lstick,		//L�X�e�B�b�N
	Shaft,		//�X�e�B�b�N�̎�
	Line,		//���C���e�������UI
	Side,		//�T�C�h�e�������UI
	Wide,		//���C�h�e�������UI
	Clear,		//�N���A
	Faild,		//���s

	Max			//�ő吔(for���Ƃ��񂷗p)
};

class UI
{
private:
	std::vector<NSprite> uiSprite_{ ((uint32_t)UIType::Max) };		//UI�`��p�X�v���C�g�Q
	std::vector<NEasing::EaseTimer> easeTimer_;						//UI�p�C�[�W���O�^�C�}�[�Q
	std::vector<NEasing::EaseTimer> keepTimer_;						//UI�p�ҋ@�^�C�}�[�Q
	std::vector<NEasing::EaseTimer> easeBackTimer_;					//UI�p�C�[�W���O�o�b�N�^�C�}�[�Q

public:
	UI();
	static UI* GetInstance();

	void Init();
	void Update();

	//�w�肳�ꂽUI��`��
	void Draw(UIType uiType);

	//�w�肳�ꂽUI�̃^�C�}�[�X�^�[�g
	void StartEaseTimer(UIType uiType) { easeTimer_[(uint32_t)uiType].Start(); }

	//�w�肳�ꂽUI�̕`����W�ݒ�
	void SetPos(UIType uiType, const NVec2& pos);
	//�w�肳�ꂽUI�̕`��T�C�Y�ݒ�
	void SetSize(UIType uiType, const NVec2& size);
	//�w�肳�ꂽUI�̕`���_�ݒ�
	void SetAncorPoint(UIType uiType, const NVec2& ancorPoint);
	//�w�肳�ꂽUI�̕`��F�ݒ�
	void SetColor(UIType uiType, const NColor& color);
	//�w�肳�ꂽUI�̔�\���t���O�ݒ�
	void SetInvisible(UIType uiType, bool isInvisible);
};