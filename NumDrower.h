#pragma once
#include "NSprite.h"

class NumDrower
{
private:
	static std::vector<std::unique_ptr<NSprite>> numSprite_;
	static NVector2 pos_;					//���W�ۑ��p
	static NVector2 size_;					//�T�C�Y�ۑ��p
	static std::vector<uint32_t> digit_;	//�e���̐����i�[�p
	static const float scale_;				//�����̑傫��

public:
	//����
	//spriteNum:�m�ۂ���X�v���C�g�̐�
	static void Create(uint32_t spriteNum);
	static void Update();
	static void Draw();

	static void SetPos(const NVector2& pos);
	static void SetSize(const NVector2& size);
	static void SetNum(uint32_t num);
};