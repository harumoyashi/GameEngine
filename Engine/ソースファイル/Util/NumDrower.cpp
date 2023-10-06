#include "NumDrower.h"

void NumDrower::Create(uint32_t spriteNum, float indent)
{
	for (size_t i = 0; i < spriteNum; i++)
	{
		if (numSprite_.size() < spriteNum)
		{
			numSprite_.emplace_back();
			numSprite_[i] = std::make_unique<NSprite>();
			//アンカーポイントは左上
			numSprite_[i]->CreateClipSprite("number", { 0, 0 }, { scale_,scale_ }, { 0,0 });
		}
	}

	indent_ = indent;
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

void NumDrower::SetPos(const NVec2& pos)
{
	pos_ = pos;
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		float posX = pos.x + numSprite_[i]->GetSize().x * indent_ * (float)i;
		numSprite_[i]->SetPos(posX, pos_.y);
	}
}

void NumDrower::SetSize(const NVec2& size)
{
	size_ = size;
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		numSprite_[i]->SetSize(size_.x, size_.y);
	}
}

void NumDrower::SetIndent(float indent)
{
	indent_ = indent;
	SetPos(pos_);
}

void NumDrower::SetColor(const NColor& color)
{
	color_ = color;
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		numSprite_[i]->color_ = color_;
	}
}

void NumDrower::SetNum(uint32_t num)
{
	uint32_t result = num;	//影響出ないように別の変数に格納
	uint32_t divide = (uint32_t)pow(10, numSprite_.size() - 1);	//最大桁数求める

	//桁数分回す
	for (size_t i = 0; i < numSprite_.size(); i++)
	{
		digit_.emplace_back();
		digit_[i] = result / divide;  //各桁ごとの値を順に格納
		result = result % divide;     //格納した桁はバイバイ
		divide /= 10;                 //次の計算のために桁落とす
		//対応したテクスチャを設定しなおす
		numSprite_[i]->CreateClipSprite("number", { scale_ * digit_[i], 0 }, { scale_,scale_ }, { 0,0 });
		//保存してた設定を反映
		float posX = pos_.x + numSprite_[i]->GetSize().x * indent_ * (float)i;
		numSprite_[i]->SetPos(posX, pos_.y);
		numSprite_[i]->SetSize(size_.x, size_.y);
	}
}