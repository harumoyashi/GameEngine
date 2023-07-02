#include "Field.h"
#include "Player.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	linePos_ = -kStartPos * NWindows::kWin_height;
	isStart = false;
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
	linePos_ = -kStartPos * NWindows::kWin_height +
		Player::GetInstance()->GetPos().z * NWindows::kWin_height +
		NWindows::kWin_height / 2.0f;

	obj_->Update();

	for (auto& sprite : sprites_)
	{
		sprite->Update();
	}

	sprites_[(uint32_t)SpriteType::Line]->SetPos(0 + slidePos_, linePos_);
	sprites_[(uint32_t)SpriteType::Start]->SetPos(1000.0f + slidePos_, linePos_ + 10.0f);

	//線を超えたらスタートした判定trueに
	if (kStartPos <= Player::GetInstance()->GetPos().z)
	{
		isStart = true;
	}

	//スタートしたなら
	if (isStart)
	{
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
		}

		slideTimer_.Update();

		if (slideTimer_.GetEnd())
		{
			//画面外まで行ったならスプライト消す
			sprites_.erase(sprites_.begin() + (uint32_t)SpriteType::Start);
		}
		//画面左外までぶっ飛ばす
		slidePos_ = NEasing::InQuad(0.0f, -(float)NWindows::kWin_width, slideTimer_.GetTimeRate());
	}
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
