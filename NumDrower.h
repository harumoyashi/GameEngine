#pragma once
#include "NSprite.h"

class NumDrower final
{
private:
	std::vector<std::unique_ptr<NSprite>> numSprite_;
	NVector2 pos_;					//���W�ۑ��p
	NVector2 size_;					//�T�C�Y�ۑ��p
	NColor color_;					//�F�ۑ��p
	std::vector<uint32_t> digit_;	//�e���̐����i�[�p
	const float scale_ = 64.f;		//�����̑傫��

public:
	//����
	//spriteNum:�m�ۂ���X�v���C�g�̐�
	void Create(uint32_t spriteNum);
	void Update();
	void Draw();

	// �Q�b�^�[ //
	NVector2 GetPos()const { return pos_; }
	NVector2 GetSize()const { return size_; }

	// �Z�b�^�[ //
	void SetPos(const NVector2& pos);
	void SetSize(const NVector2& size);
	void SetColor(const NColor& color);
	void SetNum(uint32_t num);
};