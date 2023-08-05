#include "NumDrower.h"

std::vector<std::unique_ptr<NSprite>> NumDrower::numSprite_;
const float NumDrower::scale_ = 64.f;
NVector2 NumDrower::pos_ = { 0.f,0.f };
NVector2 NumDrower::size_ = { scale_,scale_ };
std::vector<uint32_t> NumDrower::digit_;

void NumDrower::Create(uint32_t spriteNum)
{
	for (size_t i = 0; i < spriteNum; i++)
	{
		numSprite_.emplace_back();
		numSprite_[i] = std::make_unique<NSprite>();
		//�A���J�[�|�C���g�͍���
		numSprite_[i]->CreateClipSprite("number", { 0, 0 }, { scale_,scale_ }, { 0,0 });
	}
}

void NumDrower::Update()
{
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		numSprite_[i]->Update();
	}
}

void NumDrower::Draw()
{
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		numSprite_[i]->Draw();
	}
}

void NumDrower::SetPos(const NVector2& pos)
{
	pos_ = pos;
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		float posX = pos.x + numSprite_[i]->GetSize().x * 0.8f * (float)i;
		numSprite_[i]->SetPos(posX, pos_.y);
	}
}

void NumDrower::SetSize(const NVector2& size)
{
	size_ = size;
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		numSprite_[i]->SetSize(size_.x, size_.y);
	}
}

void NumDrower::SetNum(uint32_t num)
{
	uint32_t result = num;	//�e���o�Ȃ��悤�ɕʂ̕ϐ��Ɋi�[
	uint32_t divide = (uint32_t)pow(10, numSprite_.size() - 1);	//�ő包�����߂�

	//��������
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		digit_.emplace_back();
		digit_[i] = result / divide;  //�e�����Ƃ̒l�����Ɋi�[
		result = result % divide;     //�i�[�������̓o�C�o�C
		divide /= 10;                 //���̌v�Z�̂��߂Ɍ����Ƃ�
		//�Ή������e�N�X�`����ݒ肵�Ȃ���
		numSprite_[i]->CreateClipSprite("number", { scale_ * digit_[i], 0 }, { scale_,scale_ }, { 0,0 });
		//�ۑ����Ă��ݒ�𔽉f
		float posX = pos_.x + numSprite_[i]->GetSize().x * 0.8f * (float)i;
		numSprite_[i]->SetPos(posX, pos_.y);
		numSprite_[i]->SetSize(size_.x, size_.y);
	}
}