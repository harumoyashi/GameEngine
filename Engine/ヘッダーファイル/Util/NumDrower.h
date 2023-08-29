#pragma once
#include "NSprite.h"

class NumDrower final
{
private:
	std::vector<std::unique_ptr<NSprite>> numSprite_;
	NVec2 pos_;						//���W�ۑ��p
	NVec2 size_;					//�T�C�Y�ۑ��p
	NColor color_;					//�F�ۑ��p
	std::vector<uint32_t> digit_;	//�e���̐����i�[�p
	const float scale_ = 64.f;		//�����̑傫��
	float indent_;					//�������m�̕�

public:
	//����
	//spriteNum:�m�ۂ���X�v���C�g�̐�
	void Create(uint32_t spriteNum, float indent = 0.8f);
	void Update();
	void Draw();

	// �Q�b�^�[ //
	NVec2 GetPos()const { return pos_; }
	NVec2 GetSize()const { return size_; }

	// �Z�b�^�[ //
	void SetPos(const NVec2& pos);
	void SetSize(const NVec2& size);
	void SetColor(const NColor& color);
	void SetNum(uint32_t num);
};