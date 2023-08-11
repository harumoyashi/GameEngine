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
	isStart_ = false;
	isGoal_ = false;

#pragma region オブジェクトの生成
	fieldObj_ = std::make_unique<NTile>();
	fieldObj_->Init();
	fieldObj_->SetModel("plane");
	fieldObj_->SetTexture("tile");
	fieldObj_->color_.SetColor255(5, 5, 5, 255);
	fieldObj_->scale_ = { 100.0f,0.01f,1000.0f };
	fieldObj_->position_ = { 0,-0.1f,fieldObj_->scale_.z - 100.0f };
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);

	lines_.clear();	//一回全部消してから生成し直す
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_.emplace_back();
		lines_.back().line = std::make_unique<NObj3d>();
		lines_.back().line->Init();
		lines_.back().line->SetModel("plane");
		lines_.back().line->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
		lines_.back().text = std::make_unique<NObj3d>();
		lines_.back().text->Init();
		lines_.back().text->SetModel("plane");
		lines_.back().text->scale_ = { 1.5f,1.0f,0.25f };

		lines_.back().isSlide = false;
		lines_.back().slidePos = 0.0f;
		lines_.back().slideTimer.Reset();
	}

	checkPoints_.clear();	//一回全部消してから生成し直す
	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_.emplace_back();
		checkPoints_.back().line = std::make_unique<NObj3d>();
		checkPoints_.back().line->Init();
		checkPoints_.back().line->SetModel("plane");
		checkPoints_.back().line->scale_ = { fieldObj_->scale_.x * 0.1f,1.0f, 0.05f };
		checkPoints_.back().text = std::make_unique<NObj3d>();
		checkPoints_.back().text->Init();
		checkPoints_.back().text->SetModel("plane");
		checkPoints_.back().text->scale_ = { 2.5f,1.0f,0.25f };
		checkPoints_.back().text->SetTexture("checkpoint");

		checkPoints_.back().isSlide = false;
		checkPoints_.back().slidePos = 0.0f;
		checkPoints_.back().slideTimer.Reset();
	}
#pragma endregion
#pragma region 各オブジェクトの設定
	lines_[(uint32_t)LineType::Start].text->SetTexture("start");
	lines_[(uint32_t)LineType::Start].offset = 6.0f;
	lines_[(uint32_t)LineType::Start].text->position_ = { lines_[(uint32_t)LineType::Start].offset,0, startPosZ_ - 0.5f };
	lines_[(uint32_t)LineType::Start].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Start].slidePos,0, startPosZ_
	};
	lines_[(uint32_t)LineType::Start].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Start].offset +
		lines_[(uint32_t)LineType::Start].slidePos,
		0, startPosZ_ - 0.5f
	};

	lines_[(uint32_t)LineType::Goal].text->SetTexture("goal");
	lines_[(uint32_t)LineType::Goal].offset = 6.0f;
	lines_[(uint32_t)LineType::Goal].text->position_ = { lines_[(uint32_t)LineType::Goal].offset,0, goalPosZ_ - 0.5f };
	lines_[(uint32_t)LineType::Goal].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Goal].slidePos,0, goalPosZ_
	};
	lines_[(uint32_t)LineType::Goal].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Goal].offset +
		lines_[(uint32_t)LineType::Goal].slidePos,
		0, goalPosZ_ - 0.5f
	};
#pragma endregion
}

void Field::Update()
{
#pragma region 座標を適用
	lines_[(uint32_t)LineType::Start].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Start].slidePos,0, startPosZ_
	};
	lines_[(uint32_t)LineType::Start].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Start].offset +
		lines_[(uint32_t)LineType::Start].slidePos,
		0, startPosZ_ - 0.5f
	};

	lines_[(uint32_t)LineType::Goal].line->position_ =
	{
		Player::GetInstance()->GetPos().x + lines_[(uint32_t)LineType::Goal].slidePos,0, goalPosZ_
	};
	lines_[(uint32_t)LineType::Goal].text->position_ =
	{
		Player::GetInstance()->GetPos().x +
		lines_[(uint32_t)LineType::Goal].offset +
		lines_[(uint32_t)LineType::Goal].slidePos,
		0, goalPosZ_ - 0.5f
	};

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		//スタートからゴールまでをn+1等分して、1~n-1番目の線にスタート位置足した座標をチェックポイントにする
		float posZ = startPosZ_ + (goalPosZ_ - startPosZ_) / (float)(checkPointNum + 1) * (float)(i + 1);
		checkPoints_[i].line->position_ =
		{
			Player::GetInstance()->GetPos().x + checkPoints_[i].slidePos,0, posZ
		};
		checkPoints_[i].text->position_ =
		{
			Player::GetInstance()->GetPos().x +
			checkPoints_[i].offset +
			checkPoints_[i].slidePos,
			0, posZ - 0.5f
		};
	}
#pragma endregion 
	//スタートしてないとき
	if (isStart_ == false)
	{
		//線を超えたらスタートした判定trueに
		if (startPosZ_ <= Player::GetInstance()->GetPos().z)
		{
			isStart_ = true;
			lines_[(uint32_t)LineType::Start].isSlide = true;
		}
	}

	//スタートしたなら
	if (isStart_)
	{
		//画面左外までぶっ飛ばす
		lines_[(uint32_t)LineType::Start].slidePos =
			NEasing::InQuad(0.0f,
				-fieldObj_->scale_.x,
				lines_[(uint32_t)LineType::Start].slideTimer.GetTimeRate());
		//画面左外までぶっ飛ばす
		for (uint32_t i = 0; i < checkPointNum; i++)
		{
			checkPoints_[i].slidePos =
				NEasing::InQuad(0.0f, -fieldObj_->scale_.x, checkPoints_[i].slideTimer.GetTimeRate());
		}

		//スライドしきったら
		if (lines_[(uint32_t)LineType::Start].slideTimer.GetEnd())
		{
		}

		//線を超えたらスタートした判定trueに
		for (uint32_t i = 0; i < checkPointNum; i++)
		{
			//スタートからゴールまでをn+1等分して、1~n-1番目の線にスタート位置足した座標をチェックポイントにする
			float posZ = startPosZ_ + (goalPosZ_ - startPosZ_) / (float)(checkPointNum + 1) * (float)(i + 1);
			if (posZ <= Player::GetInstance()->GetPos().z)
			{
				checkPoints_[i].isSlide = true;
			}
		}

		//------------------------------------- 敵の生成処理 -------------------------------------//
		//敵出しちゃうよボタン
		if (NInput::IsKeyDown(DIK_C))
		{
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
		}
	}

	//ゴールしてないとき
	if (isGoal_ == false)
	{
		//線を超えたらスタートした判定trueに
		if (goalPosZ_ <= Player::GetInstance()->GetPos().z)
		{
			isGoal_ = true;
			lines_[(uint32_t)LineType::Goal].isSlide = true;
		}
	}

	//ゴールしたなら
	if (isGoal_)
	{
		//画面左外までぶっ飛ばす
		lines_[(uint32_t)LineType::Goal].slidePos =
			NEasing::InQuad(0.0f,
				-fieldObj_->scale_.x,
				lines_[(uint32_t)LineType::Goal].slideTimer.GetTimeRate());

		//スライドしきったら
		if (lines_[(uint32_t)LineType::Goal].slideTimer.GetEnd())
		{
		}
	}

	//スライドしていいなら
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		if (lines_[i].isSlide)
		{
			//スタート線スライドタイマー開始
			if (lines_[i].slideTimer.GetStarted() == false)
			{
				lines_[i].slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5));

				NAudioManager::Play("startSE");
				lines_[i].isSlide = false;	//スライドしちゃだめにする
			}
		}
		//タイマー更新
		lines_[i].slideTimer.Update();
	}
	//スライドしていいなら
	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		if (checkPoints_[i].isSlide)
		{
			//スタート線スライドタイマー開始
			if (checkPoints_[i].slideTimer.GetStarted() == false)
			{
				checkPoints_[i].slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8));
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5));

				NAudioManager::Play("startSE");
				checkPoints_[i].isSlide = false;	//スライドしちゃだめにする
			}
		}
		//タイマー更新
		checkPoints_[i].slideTimer.Update();
	}

	//オブジェクトの更新
	fieldObj_->SetDivide(tileDivide_);
	fieldObj_->SetActivityArea(activityAreaX_);
	fieldObj_->Update();

	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_[i].line->Update();
		lines_[i].text->Update();
	}

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_[i].line->Update();
		checkPoints_[i].text->Update();
	}

#ifdef _DEBUG
	ImGui::Begin("FieldParameter");
	//1F~180Fまでの間にとどめる
	ImGui::SliderFloat("StartPosZ", &startPosZ_, 1.0f, 180.0f);
	ImGui::SliderFloat("GoalPosZ", &goalPosZ_, 1.0f, 180.0f);
	ImGui::SliderFloat("Divide", &tileDivide_, 0.0f, 10.0f);
	ImGui::SliderFloat("ActivityArea", &activityAreaX_, 1.0f, 100.0f);
	ImGui::End();
#endif //DEBUG
}

void Field::Draw()
{
	//床だけタイリングする
	NTile::CommonBeginDraw();
	fieldObj_->Draw();
	NObj3d::CommonBeginDraw();

	NObj3d::SetBlendMode(BlendMode::Alpha);
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_[i].line->Draw();
		lines_[i].text->Draw();
	}

	for (uint32_t i = 0; i < checkPointNum; i++)
	{
		checkPoints_[i].line->Draw();
		checkPoints_[i].text->Draw();
	}
	NObj3d::SetBlendMode(BlendMode::None);
}