#include "Field.h"
#include "Player.h"
#include "EnemyFactory.h"
#include "NCollisionManager.h"
#include "NAudioManager.h"

#include <functional>
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
	for (uint32_t i = 0; i < 2; i++)
	{
		if (fieldObj_[i] == nullptr)
		{
			fieldObj_[i] = std::make_unique<NTile>();
			fieldObj_[i]->Init();
			fieldObj_[i]->SetModel("field");
			fieldObj_[i]->SetTexture("tile");
		}
		fieldObj_[i]->color_.SetColor255(5, 5, 5, 255);
		fieldObj_[i]->scale_ = { 50.0f,0.01f,50.0f };
		fieldObj_[i]->position_ = { 0,-0.1f,fieldObj_[i]->scale_.z * (float)i };
		fieldObj_[i]->SetDivide(tileDivide_);
		fieldObj_[i]->SetActivityArea(activityAreaX_);

		//コライダー設定
		collider_[i].SetNormal(NVec3::up);
		collider_[i].SetDistance(0.f);
		collider_[i].SetColID("field");
		NCollisionManager::GetInstance()->AddCollider(&collider_[i]);
		collider_[i].SetOnCollision(std::bind(&Field::OnCollision, this));
	}

	lines_.clear();	//一回全部消してから生成し直す
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_.emplace_back();
		lines_.back().line = std::make_unique<NObj3d>();
		lines_.back().line->Init();
		lines_.back().line->SetModel("plane");
		lines_.back().line->scale_ = { fieldObj_[0]->scale_.x * 0.1f,1.0f, 0.05f };
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
		checkPoints_.back().line->scale_ = { fieldObj_[0]->scale_.x * 0.1f,1.0f, 0.05f };
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
	//床のスクロール処理
	for (uint32_t i = 0; i < 2; i++)
	{
		//プレイヤーと床の距離
		float toPlayerLen = fieldObj_[i]->position_.z - Player::GetInstance()->GetPos().z;
		//プレイヤーと床が床のサイズより離れたら
		if (abs(toPlayerLen) > fieldObj_[i]->scale_.z)
		{
			//1枚目の場合2枚目基準にずらす
			if (i == 0)
			{
				fieldObj_[i]->position_.z =
					fieldObj_[1]->position_.z + fieldObj_[i]->scale_.z * -MathUtil::Signf(toPlayerLen);
			}
			//2枚目の場合1枚目基準にずらす
			else
			{
				fieldObj_[i]->position_.z =
					fieldObj_[0]->position_.z + fieldObj_[i]->scale_.z * -MathUtil::Signf(toPlayerLen);
			}
		}
	}

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
				-fieldObj_[0]->scale_.x,
				lines_[(uint32_t)LineType::Start].slideTimer.GetTimeRate());
		//画面左外までぶっ飛ばす
		for (auto& cp : checkPoints_)
		{
			cp.slidePos =
				NEasing::InQuad(0.0f, -fieldObj_[0]->scale_.x, cp.slideTimer.GetTimeRate());
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
#ifdef _DEBUG
		//敵出しちゃうよボタン
		if (NInput::IsKeyDown(DIK_C))
		{
			EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8), true);
		}
#endif
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
				-fieldObj_[0]->scale_.x,
				lines_[(uint32_t)LineType::Goal].slideTimer.GetTimeRate());

		//スライドしきったら
		if (lines_[(uint32_t)LineType::Goal].slideTimer.GetEnd())
		{
		}
	}

	//スライドしていいなら
	for (auto& line : lines_)
	{
		if (line.isSlide)
		{
			//スタート線スライドタイマー開始
			if (line.slideTimer.GetStarted() == false)
			{
				line.slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8), true);
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5), false);
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseFront, Player::GetInstance()->GetPos() + NVec3(0, 0, 9), false);

				NAudioManager::GetInstance()->Play("startSE");
				line.isSlide = false;	//スライドしちゃだめにする
			}
		}
		//タイマー更新
		line.slideTimer.Update();
	}
	//スライドしていいなら
	for (auto& cp : checkPoints_)
	{
		if (cp.isSlide)
		{
			//スタート線スライドタイマー開始
			if (cp.slideTimer.GetStarted() == false)
			{
				cp.slideTimer.Start();
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(10, 0, 8), false);
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseSide, Player::GetInstance()->GetPos() + NVec3(-10, 0, 5), true);
				EnemyFactory::GetInstance()->Create(IEnemy::EnemyType::MouseFront, Player::GetInstance()->GetPos() + NVec3(0, 0, 9), false);

				NAudioManager::GetInstance()->Play("startSE");
				cp.isSlide = false;	//スライドしちゃだめにする
			}
		}
		//タイマー更新
		cp.slideTimer.Update();
	}

	//オブジェクトの更新
	for (auto& field : fieldObj_)
	{
		field->SetDivide(tileDivide_);
		field->SetActivityArea(activityAreaX_);
		field->Update();
	}

	for (auto& line : lines_)
	{
		line.line->Update();
		line.text->Update();
	}

	for (auto& cp : checkPoints_)
	{
		cp.line->Update();
		cp.text->Update();
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
	for (auto& field : fieldObj_)
	{
		field->Draw();
	}

	NObj3d::CommonBeginDraw();
	for (auto& line : lines_)
	{
		line.line->SetBlendMode(BlendMode::Alpha);
		line.line->Draw();
		line.text->Draw();
		line.text->SetBlendMode(BlendMode::None);
	}

	for (auto& cp : checkPoints_)
	{
		cp.line->SetBlendMode(BlendMode::Alpha);
		cp.line->Draw();
		cp.text->Draw();
		cp.text->SetBlendMode(BlendMode::None);
	}
}

void Field::OnCollision()
{
}