#include "Field.h"
#include "Player.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	linePos_ = -startPos_ * NWindows::kWin_height;
#pragma region オブジェクトの生成,設定
	obj_ = std::make_unique<NObj3d>();
	obj_->Init();
	obj_->SetModel("cube");
	obj_->color_.SetColor255(5, 5, 5, 255);
	obj_->scale_ = { 100.0f,0.1f,5000.0f };
	obj_->position_ = { 0,1.0f,obj_->scale_.z - 100.0f };
#pragma endregion
#pragma region スプライトの生成
	for (uint32_t i = 0; i < (uint32_t)SpriteType::MaxSize; i++)
	{
		sprites_.emplace_back();
		sprites_.back() = std::make_unique<NSprite>();
	}
#pragma endregion
#pragma region 各スプライトの設定
	sprites_[(uint32_t)SpriteType::Line]->CreateSprite("white", { 0.f,0.f });
	sprites_[(uint32_t)SpriteType::Line]->SetSize(NWindows::kWin_width, 10.0f);
	sprites_[(uint32_t)SpriteType::Line]->SetPos(0, linePos_);

	sprites_[(uint32_t)SpriteType::Start]->CreateSprite("start", { 0.f,0.f });
	sprites_[(uint32_t)SpriteType::Start]->SetPos(1000.0f, linePos_ + 10.0f);
#pragma endregion
}

void Field::Update()
{
	linePos_ = -startPos_ * NWindows::kWin_height +
		Player::GetInstance()->GetPos().z * NWindows::kWin_height +
		NWindows::kWin_height / 2.0f;

	obj_->Update();

	for (auto& sprite : sprites_)
	{
		sprite->Update();
	}
	sprites_[(uint32_t)SpriteType::Line]->SetPos(0, linePos_);
	sprites_[(uint32_t)SpriteType::Start]->SetPos(1000.0f, linePos_ + 10.0f);
}

void Field::DrawObj()
{
	obj_->Draw();
}

void Field::DrawSprite()
{
	for (auto& sprite : sprites_)
	{
		sprite->Draw();
	}
}
