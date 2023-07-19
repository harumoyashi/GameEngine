#include "Field.h"
#include "Player.h"
#include "EnemyFactory.h"

#include "NAudioManager.h"

#include "NImGuiManager.h"
#include "imgui.h"

Field* Field::GetInstance()
{
	static Field instance;
	return &instance;
}

void Field::Init()
{
	linePosZ_ = kStartPosZ;
	isStart_ = false;
	startOffset_ = 5.0f;
	slidePos_ = 0.0f;
	slideTimer_.Reset();
	isStart_ = false;

#pragma region オブジェクトの生成
	fieldObj_ = std::make_unique<NTile>();
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
	fieldObj_->scale_ = { 100.0f,0.01f,1000.0f };
	fieldObj_->position_ = { 0,-0.1f,fieldObj_->scale_.z - 100.0f };
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);

	obj_[(uint32_t)ObjType::Line]->SetModel("plane");
	obj_[(uint32_t)ObjType::Line]->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
	obj_[(uint32_t)ObjType::Line]->position_ = { 0,0, linePosZ_ };

	obj_[(uint32_t)ObjType::Start]->SetModel("plane");
	obj_[(uint32_t)ObjType::Start]->model_.material.texture = NTextureManager::GetInstance()->textureMap_["start"];
	obj_[(uint32_t)ObjType::Start]->scale_ = { 1.5f,1.0f,0.25f };	//縦横比6:1
	obj_[(uint32_t)ObjType::Start]->position_ = { startOffset_,0, linePosZ_ - 0.5f };
#pragma endregion
}

void Field::Update()
{
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);
	fieldObj_->Update();

	for (auto& obj : obj_)
	{
		obj->Update();
	}

	obj_[(uint32_t)ObjType::Line]->position_ =
	{ Player::GetInstance()->GetPos().x + slidePos_,0, linePosZ_ };
	if (slideTimer_.GetEnd() == false)
	{
		obj_[(uint32_t)ObjType::Start]->position_ =
		{ Player::GetInstance()->GetPos().x + startOffset_ + slidePos_,0, linePosZ_ - 0.5f };
	}

	//線を超えたらスタートした判定trueに
	if (linePosZ_ <= Player::GetInstance()->GetPos().z)
	{
		isStart_ = true;
	}

	//スタートしたなら
	if (isStart_)
	{
		if (slideTimer_.GetStarted() == false)
		{
			slideTimer_.Start();
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::WolfSide, Player::GetInstance()->GetPos() + NVector3(10,0,8));
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::WolfSide, Player::GetInstance()->GetPos() + NVector3(-10,0,5));

			NAudioManager::Play("startSE");
		}

		//敵出しちゃうよボタン
		if (NInput::IsKeyDown(DIK_C))
		{
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::WolfSide, Player::GetInstance()->GetPos() + NVector3(10, 0, 8));
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


		//------------------------------------- 敵の生成処理 -------------------------------------//
		//EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::WolfSide,{7,0,8});
	}

	//リリースでもいじりたいからifdefで囲ってない
	ImGui::Begin("FieldParameter");
	//1F~180Fまでの間にとどめる
	ImGui::SliderFloat("LinePosZ", &linePosZ_, 1.0f, 180.0f);
	ImGui::SliderFloat("Divide", &tileDivide_, 0.0f, 10.0f);
	ImGui::SliderFloat("ActivityArea", &activityAreaX_, 1.0f, 100.0f);
	ImGui::End();
}

void Field::Draw()
{
	//床だけタイリングする
	NTile::CommonBeginDraw();
	fieldObj_->Draw();
	NObj3d::CommonBeginDraw();

	for (auto& obj : obj_)
	{
		obj->Draw();
	}
}