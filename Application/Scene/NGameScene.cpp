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
#include "Field.h"
#include "Wave.h"

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
	Field::GetInstance()->Init();
	Wave::GetInstance()->Init();

#pragma region オブジェクトの初期値設定


#pragma endregion
	//背景スプライト生成

	//前景スプライト生成
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i] = std::make_unique<NSprite>();
	}
	foreSprite_[(uint32_t)FSpriteType::Shaft]->CreateSprite("shaft");
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::Shaft]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 200.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->CreateSprite("stick");
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetSize(100.0f, 100.0f);
	foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(NWindows::GetInstance()->kWin_width * 0.5f, 200.0f);

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
}

void NGameScene::Update()
{
#pragma region カメラ
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region スプライト
	for (uint32_t i = 0; i < (uint32_t)FSpriteType::MaxForeSprite; i++)
	{
		foreSprite_[i]->Update();
	}

#pragma endregion
	BulletManager::GetInstance()->Update();
	EnemyManager::GetInstance()->Update();
	Field::GetInstance()->Update();

	NParticleManager::GetInstance()->Update();

	//ライトたちの更新
	lightGroup_->Update();

	NCollisionManager::GetInstance()->CheckAllCollision();

	if (scene == SceneMode::Play)	//プレイ中の処理
	{
		Player::GetInstance()->Update();
		Wave::GetInstance()->Update();
		//死亡パーティクル出るボタン
		if (NInput::IsKeyDown(DIK_0))
		{
			Player::GetInstance()->DeadParticle();
		}

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
				NWindows::GetInstance()->kWin_width * 0.5f + slidePos, 200.0f);
		}

		NVector2 stickVec;
		stickVec = NInput::GetInstance()->GetStick() * 10.0f;
		//Yはスティックだと上が正の値なので引く
		foreSprite_[(uint32_t)FSpriteType::LStick]->SetPos(
			NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos, 200.0f - stickVec.y);

		//プレイヤーが波に飲み込まれたら殺す
		if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
		{
			Player::GetInstance()->DeadParticle();
			Player::GetInstance()->SetIsAlive(false);
		}

		//プレイヤーが死んで、死亡演出が終わったら失敗リザルトへ
		if (Player::GetInstance()->GetIsAlive() == false &&
			Player::GetInstance()->GetDeadEffectEnd())
		{
			scene = SceneMode::Faild;
			Player::GetInstance()->FaildUpdate();	//ここでプレイヤーの座標変えてあげないとカメラの座標が死んだ座標基準になっちゃう
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Faild);
		}

		if (Field::GetInstance()->GetIsGoal())
		{
			scene = SceneMode::Clear;
			NCameraManager::GetInstance()->ChangeCameara(CameraType::Clear);
		}
	}
	else if (scene == SceneMode::Clear)	//クリアリザルトの処理
	{
		Player::GetInstance()->ClearUpdate();

		//シーン切り替え
		if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
		{
			NSceneChange::GetInstance()->Start();	//シーン遷移開始
		}
	}
	else if (scene == SceneMode::Faild)	//失敗リザルトの処理
	{
		Player::GetInstance()->FaildUpdate();

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
}

void NGameScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
	Wave::GetInstance()->Draw();
}

void NGameScene::DrawBackSprite()
{
}

void NGameScene::Draw3D()
{
	BulletManager::GetInstance()->Draw();
	EnemyManager::GetInstance()->Draw();
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
}