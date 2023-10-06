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
	Clear,		//�N���A
	Faild,		//���s

	Max			//�ő吔(for���Ƃ��񂷗p)
};

struct UI
{
	NSprite sprite;						//UI�`��p�X�v���C�g�Q
	NEasing::EaseTimer easeTimer;		//UI�p�C�[�W���O�^�C�}�[�Q
	NEasing::EaseTimer keepTimer;		//UI�p�ҋ@�^�C�}�[�Q
	NEasing::EaseTimer easeBackTimer;	//UI�p�C�[�W���O�o�b�N�^�C�}�[�Q
	NVec2 startPos;						//�n�_���W
	NVec2 endPos;						//�I�_���W
	bool isActive;						//�L���t���O
};

class UIManager
{
private:
	std::vector<UI> ui_{ ((uint32_t)UIType::Max) };	//UI�Q
	const uint32_t maxUIBul = 3;
	std::vector<UI> uiBul_{ (maxUIBul) };	//�e���������UI�Q

public:
	UIManager();
	static UIManager* GetInstance();

	void Init();
	void Update();
	void EaseTimerUpdate();

	//�e�������UI�ǉ�
	void PlusUIBul(const std::string& texName);

	//�w�肳�ꂽUI��`��
	void Draw(UIType uiType);
	//�e�Ƃ�������UI�`��
	void DrawUIBul();

	//�w�肳�ꂽUI�̃^�C�}�[�X�^�[�g
	void StartEaseTimer(UIType uiType) { ui_[(uint32_t)uiType].easeTimer.Start(); }

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
	//�w�肳�ꂽUI�̃e�N�X�`���ݒ�
	void SetTexture(UIType uiType, const std::string& texName);
	//�w�肳�ꂽUI�̗L���t���O�ݒ�
	void SetIsActive(UIType uiType, bool isActive);
};