#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NAudioManager.h"
#include "NParticleManager.h"
#include "NCollisionManager.h"
#include "NPostEffect.h"

#include "Player.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Field.h"
#include "Wave.h"
#include "Score.h"

SceneMode NGameScene::scene = SceneMode::Play;

void NGameScene::LoadResources()
{
}

void NGameScene::Init()
{
#pragma region	オーディオ初期化
	NAudio::GetInstance()->Init();
	NAudioManager::Play("playBGM", true, 0.2f);
#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region 描画初期化処理
	//オブジェクト
	NParticleManager::GetInstance()->Init();
	NCollisionManager::GetInstance()->Init();
	Player::GetInstance()->Init();
	BulletManager::GetInstance()->Init();
	EnemyManager::GetInstance()->Init();
	ItemManager::GetInstance()->Init();
	Field::GetInstance()->Init();
	Wave::GetInstance()->Init();
#pragma region オブジェクトの初期値設定


#pragma endregion
	//背景スプライト生成
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("white");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	backSprite_->color_.SetColor255(10, 10, 10);

	//前景スプライト生成
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i] = std::make_unique<NSprite>();
	}

	Score::Init();
#pragma region 各スプライトの設定
	foreSprite_[(uint32_t)FSpriteType::Shaft]->CreateSprite("shaft");
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 500.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->CreateSprite("stick");
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 500.0f);

	for (uint32_t i = 0; i < 2; i++)
	{
		foreSprite_[(uint32_t)FSpriteType::Abutton + i] = std::make_unique<NSprite>();
		foreSprite_[(uint32_t)FSpriteType::Abutton + i]->CreateClipSprite("Abutton", { i * 192.f,0 }, { 192.f,192.f });
		foreSprite_[(uint32_t)FSpriteType::Abutton + i]->SetPos(
			(float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f);
	}

	foreSprite_[(uint32_t)FSpriteType::Clear]->CreateSprite("clear");
	foreSprite_[(uint32_t)FSpriteType::Clear]->SetPos(-(float)NWindows::GetInstance()->kWin_width, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Clear]->SetSize(350.f, 100.f);
	foreSprite_[(uint32_t)FSpriteType::Faild]->CreateSprite("faild");
	foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(-(float)NWindows::GetInstance()->kWin_width, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Faild]->SetSize(350.f, 100.f);
#pragma endregion
#pragma endregion
	// ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());

	IEmitter3D::SetLightGroup(lightGroup_.get());

	NPostEffect::SetIsActive(false);	//ポストエフェクト消す

	scene = SceneMode::Play;

	slidePos = 0.0f;
	slideTimer.Reset();
	slideTimer = 0.1f;
}

void NGameScene::Update()
{
#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region スプライト
	backSprite_->Update();
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Update();
	}
	Score::Update();
#pragma endregion
	BulletManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();
	ItemManager::GetInstance()->Update();
	Field::GetInstance()->Update();
	Wave::GetInstance()->Update();

	NParticleManager::GetInstance()->Update();

	//ライトたちの更新
	lightGroup_->Update();

	if (scene == SceneMode::Play)	//プレイ中の処理
	{
		Player::GetInstance()->Update();
		//死亡パーティクル出るボタン
		if (NInput::IsKeyDown(DIK_0))
		{
			Player::GetInstance()->DeadParticle();
		}

		//Aボタンはプレイ中使わないから見せない
		foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
		foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;

		if (Field::GetInstance()->GetIsStart())
		{
			//スタート線スライドタイマー開始
			if (slideTimer.GetStarted() == false)
			{
				slideTimer.Start();
			}
			slideTimer.Update();

			slidePos = NEasing::InQuad(0.0f, -(float)NWindows::GetInstance()->kWin_width, slideTimer.GetTimeRate());
			foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(
				NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 500.0f);
		}

		NVec2 stickVec;
		stickVec = NInput::GetInstance()->GetStick() * 8.0f;
		//Yはスティックだと上が正の値なので引く
		foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(
			NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos, 500.0f - stickVec.y);

		if (Field::GetInstance()->GetIsStart())
		{
			//プレイヤーが波に飲み込まれたら殺す
			if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
			{
				Player::GetInstance()->SetIsAlive(false);
			}
		}

		//プレイヤーが死んで、死亡演出が終わったら失敗リザルトへ
		if (Player::GetInstance()->GetIsAlive() == false &&
			Player::GetInstance()->GetDeadEffectEnd())
		{
			NAudioManager::Destroy("playBGM");
			NAudioManager::Play("faildBGM", true, 0.2f);
			scene = SceneMode::Faild;
			slideTimer.Reset();
			slideTimer = 0.5f;
			Player::GetInstance()->FaildUpdate();	//ここでプレイヤーの座標変えてあげないとカメラの座標が死んだ座標基準になっちゃう
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Faild);
		}

		if (Field::GetInstance()->GetIsGoal())
		{
			NAudioManager::Destroy("playBGM");
			NAudioManager::Play("clearBGM", true, 0.2f);
			scene = SceneMode::Clear;
			slideTimer.Reset();
			slideTimer = 0.5f;
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Clear);
		}

		//当たり判定総当たり
		NCollisionManager::GetInstance()->CheckAllCollision();
	}
	else if (scene == SceneMode::Clear)	//クリアリザルトの処理
	{
		Player::GetInstance()->ClearUpdate();

		//スタート線スライドタイマー開始
		if (slideTimer.GetStarted() == false)
		{
			slideTimer.Start();
		}
		slideTimer.Update();
		//クリアテキストスライド
		slidePos = NEasing::InOutBack(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Clear]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 100.0f);

		//リザルトスコアが上から落ちてくる
		float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer.GetTimeRate());
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f - Score::GetSize(Score::TexType::Result).x * 2.f, slideP },
			Score::TexType::Result);
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f, slideP - Score::GetSize(Score::TexType::Result).y },
			Score::TexType::Top);

		//Aボタン点滅
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = false;
		}
		else
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = false;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;
		}

		//シーン切り替え
		if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
		{
			NSceneChange::GetInstance()->Start();	//シーン遷移開始
		}
	}
	else if (scene == SceneMode::Faild)	//失敗リザルトの処理
	{
		Player::GetInstance()->FaildUpdate();

		//スタート線スライドタイマー開始
		if (slideTimer.GetStarted() == false)
		{
			slideTimer.Start();
		}
		slideTimer.Update();
		//失敗テキストスライド
		slidePos = NEasing::InQuad(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer.GetTimeRate());
		foreSprite_[(uint32_t)FSpriteType::Faild]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 100.0f);

		//リザルトスコアが上から落ちてくる
		float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer.GetTimeRate());
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f - Score::GetSize(Score::TexType::Result).x * 2.f, slideP },
			Score::TexType::Result);
		Score::SetPos(
			{ NWindows::kWin_width * 0.5f, slideP - Score::GetSize(Score::TexType::Result).y },
			Score::TexType::Top);

		//Aボタン点滅
		flashingTimer_.Roop();
		if (flashingTimer_.GetTimeRate() > 0.7f)
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = true;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = false;
		}
		else
		{
			foreSprite_[(uint32_t)FSpriteType::Abutton]->isInvisible_ = false;
			foreSprite_[(uint32_t)FSpriteType::AbuttonPush]->isInvisible_ = true;
		}

		//シーン切り替え
		if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
		{
			NSceneChange::GetInstance()->Start();	//シーン遷移開始
		}
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NTitleScene>();			//タイトルシーンに切り替え
		NSceneChange::GetInstance()->SetIsChange(false);	//切り替えちゃﾀﾞﾒｰ
	}

	//シーン切り替え(デバッグ用)
	if (NInput::IsKeyDown(DIK_RETURN) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_X))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}

	//リセットボタン(デバッグ用)
	if (NInput::IsKeyDown(DIK_R) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_START))
	{
		NSceneManager::ChangeScene<NGameScene>();
	}

	//アイテム出すボタン(デバッグ用)
	if (NInput::IsKeyDown(DIK_I))
	{
		ItemManager::GetInstance()->Generate(NVec3::zero);
	}
}

void NGameScene::DrawBackSprite()
{
	backSprite_->Draw();
}

void NGameScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
	Wave::GetInstance()->Draw();
}

void NGameScene::Draw3D()
{
	BulletManager::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
	ItemManager::GetInstance()->Draw();
	Player::GetInstance()->Draw();
}

void NGameScene::DrawParticle()
{
	NParticleManager::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Draw();
	}

	Score::Draw();
}