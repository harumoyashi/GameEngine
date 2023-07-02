#include "Field.h"
#include "Player.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	linePos_ = kStartPos;
	isStart = false;
	startOffset_ = 5.0f;

#pragma region オブジェクトの生成
	fieldObj_ = std::make_unique<NObj3d>();
	fieldObj_->Init();

	for (uint32_t i = 0; i < (uint32_t)ObjType::MaxSize; i++)
	{
		obj_.emplace_back();
		obj_.back() = std::make_unique<NObj3d>();
		obj_.back()->Init();
	}
#pragma endregion
#pragma region 各オブジェクトの設定
	fieldObj_->SetModel("plane");
	fieldObj_->model_.material.texture = NTextureManager::GetInstance()->textureMap_["tile"];
	fieldObj_->color_.SetColor255(5, 5, 5, 255);
	fieldObj_->scale_ = { 100.0f,0.01f,5000.0f };
	fieldObj_->position_ = { 0,-0.1f,fieldObj_->scale_.z - 100.0f };

	obj_[(uint32_t)ObjType::Line]->SetModel("plane");
	obj_[(uint32_t)ObjType::Line]->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
	obj_[(uint32_t)ObjType::Line]->position_ = { 0,0, linePos_ };

	obj_[(uint32_t)ObjType::Start]->SetModel("plane");
	obj_[(uint32_t)ObjType::Start]->model_.material.texture = NTextureManager::GetInstance()->textureMap_["start"];
	obj_[(uint32_t)ObjType::Start]->scale_ = { 3.0f,1.0f,0.5f };	//縦横比6:1
	obj_[(uint32_t)ObjType::Start]->position_ = { startOffset_,0, linePos_ - 1.0f };
#pragma endregion
}

void Field::Update()
{
	fieldObj_->Update();
	for (auto& obj : obj_)
	{
		obj->Update();
	}

	obj_[(uint32_t)ObjType::Line]->position_ =
	{ Player::GetInstance()->GetPos().x + slidePos_,0, linePos_ };
	if (slideTimer_.GetEnd() == false)
	{
		obj_[(uint32_t)ObjType::Start]->position_ =
		{ Player::GetInstance()->GetPos().x + startOffset_ + slidePos_,0, linePos_ - 1.0f };
	}

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
			if (obj_.size() == (uint32_t)ObjType::MaxSize)
			{
				obj_.erase(obj_.begin() + (uint32_t)ObjType::Start);
			}
		}
		//画面左外までぶっ飛ばす
		slidePos_ = NEasing::InQuad(0.0f, -fieldObj_->scale_.x, slideTimer_.GetTimeRate());
	}
}

void Field::DrawObj()
{
	//床だけタイリングする
	NObj3d::CommonBeginDraw(true);
	fieldObj_->Draw();
	NObj3d::CommonBeginDraw(false);

	for (auto& obj : obj_)
	{
		obj->Draw();
	}
}

void Field::DrawSprite()
{
}
