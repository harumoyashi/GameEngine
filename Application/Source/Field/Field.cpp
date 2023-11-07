#include "Field.h"
#include "Player.h"
#include "EnemyFactory.h"
#include "NCollisionManager.h"
#include "NAudioManager.h"
#include "NCameraManager.h"

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
		//フィールドの生成
		if (fieldObj_[i] == nullptr)
		{
			fieldObj_[i] = std::make_unique<NTile>();
			fieldObj_[i]->Init();
			fieldObj_[i]->SetModel("field");
			fieldObj_[i]->SetTexture("tile");
		}
		fieldObj_[i]->color_.SetColor255(200, 200, 200, 245);
		fieldObj_[i]->scale_ = { 18.0f,0.01f,18.0f };
		fieldObj_[i]->position_ = { 0,-0.1f,fieldObj_[i]->scale_.z * (float)i };
		fieldObj_[i]->SetDivide(tileDivide_);
		fieldObj_[i]->SetActivityArea(activityAreaX_);
		fieldObj_[i]->SetAvoidArea(avoidArea_);

		//コライダー設定
		collider_[i].SetNormal(NVec3::up);
		collider_[i].SetDistance(0.f);
		collider_[i].SetColID("field");
		NCollisionManager::GetInstance()->AddCollider(&collider_[i]);
		collider_[i].SetOnCollision(std::bind(&Field::OnCollision, this));

		//背景オブジェクトの生成
		if (backObj_[i] == nullptr)
		{
			backObj_[i] = std::make_unique<BackObj>();
			backObj_[i]->Init();
			backObj_[i]->SetModel("field");
			backObj_[i]->SetTexture("white");
		}
		backObj_[i]->color_.SetColor255(150, 150, 150, 255);
		backObj_[i]->scale_ = { fieldObj_[i]->scale_.x * 1.2f,fieldObj_[i]->scale_.y,fieldObj_[i]->scale_.z * 1.2f };
		backObj_[i]->position_ = { 0,-10.f,backObj_[i]->scale_.z * (float)i };
	}

	lines_.clear();	//一回全部消してから生成し直す
	for (uint32_t i = 0; i < (uint32_t)LineType::MaxSize; i++)
	{
		lines_.emplace_back();
		lines_.back().line = std::make_unique<NObj3d>();
		lines_.back().line->Init();
		lines_.back().line->SetModel("plane");
		lines_.back().line->scale_ = { fieldObj_[0]->scale_.x * 0.5f,1.0f, 0.05f };
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
		checkPoints_.back().line->scale_ = { fieldObj_[0]->scale_.x * 0.5f,1.0f, 0.05f };
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

	extrusionTimer_.Reset();
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
	if (NCameraManager::GetInstance()->GetNowCameraType() == CameraType::BeforeStart)	//始まる前だけカメラ基準でスクロール
	{
		FieldScroll(NCamera::sCurrentCamera->GetTarget().z);
	}
	else
	{
		FieldScroll(Player::GetInstance()->GetFrontPosZ());
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
	//プレイヤー
	objPos_[0] = Player::GetInstance()->GetPos();
	//全敵
	//for (auto& enemy : EnemyManager::GetInstance()->enemys_)
	//{
	//	static uint32_t i = 1;	//プレイヤー追加してるので要素番号は1からスタート
	//	objPos_[i] = enemy->GetPos();
	//	i++;
	//}

	extrusionTimer_.RoopReverse();
	for (auto& field : fieldObj_)
	{
		field->SetDivide(tileDivide_);
		field->SetActivityArea(activityAreaX_);
		field->SetIsAvoid(isAvoid_);
		field->SetAvoidArea(avoidArea_);
		//とりあえず押し出しタイマーと同じにしてみる
		field->SetFloatingTimer(NEasing::OutQuad(extrusionTimer_.GetTimeRate()));
		for (uint32_t i = 0; i < maxObj; i++)
		{
			field->SetObjPos(objPos_[i], i);
		}
		field->Update();
	}

	for (auto& backObj : backObj_)
	{
		backObj->SetIsAvoid(isAvoid_);
		backObj->SetExtrusionTimer(NEasing::OutQuad(extrusionTimer_.GetTimeRate()));
		backObj->Update();
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
	ImGui::SliderFloat("AvoidArea", &avoidArea_, 0.0f, 10.0f);
	ImGui::SliderFloat("FieldAlpha", &fieldObj_[0]->color_.a, 0.0f, 1.0f);
	fieldObj_[1]->color_.a = fieldObj_[0]->color_.a;
	ImGui::Checkbox("IsAvoid", &isAvoid_);
	ImGui::End();
#endif //DEBUG
}

void Field::Draw()
{
	//背景オブジェクトの描画
	BackObj::CommonBeginDraw();
	for (auto& backObj : backObj_)
	{
		backObj->Draw();
	}

	//床だけタイリングする
	NTile::CommonBeginDraw(isAvoid_);
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

void Field::StopSE()
{
	NAudioManager::GetInstance()->Destroy("startSE");
}

void Field::FieldScroll(float standardPosZ)
{
	for (uint32_t i = 0; i < 2; i++)
	{
		//基準点と床の距離
		float field2PosZLen = fieldObj_[i]->position_.z - standardPosZ;
		//基準点と床が床のサイズより離れたら
		if (abs(field2PosZLen) > fieldObj_[i]->scale_.z)
		{
			//1枚目の場合2枚目基準にずらす
			if (i == 0)
			{
				fieldObj_[i]->position_.z =
					fieldObj_[1]->position_.z + fieldObj_[i]->scale_.z * -MathUtil::Signf(field2PosZLen);
			}
			//2枚目の場合1枚目基準にずらす
			else
			{
				fieldObj_[i]->position_.z =
					fieldObj_[0]->position_.z + fieldObj_[i]->scale_.z * -MathUtil::Signf(field2PosZLen);
			}
		}

		//基準点と床の距離
		float backObj2PosZLen = backObj_[i]->position_.z - standardPosZ;
		//基準点と床が床のサイズより離れたら
		if (abs(backObj2PosZLen) > backObj_[i]->scale_.z)
		{
			//1枚目の場合2枚目基準にずらす
			if (i == 0)
			{
				backObj_[i]->position_.z =
					backObj_[1]->position_.z + backObj_[i]->scale_.z * -MathUtil::Signf(backObj2PosZLen);
			}
			//2枚目の場合1枚目基準にずらす
			else
			{
				backObj_[i]->position_.z =
					backObj_[0]->position_.z + backObj_[i]->scale_.z * -MathUtil::Signf(backObj2PosZLen);
			}
		}
	}
}
