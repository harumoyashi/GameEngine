#include "NDX12.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NAudioManager.h"
#include "NParticleManager.h"
#include "NCollisionManager.h"
#include "IPostEffect.h"
#include "Bloom.h"

#include "Player.h"
#include "Boss.h"
#include "BulletManager.h"
#include "EnemyManager.h"
#include "ItemManager.h"
#include "Field.h"
#include "Wave.h"
#include "Score.h"
#include "UI.h"

SceneMode NGameScene::sScene = SceneMode::Play;

void NGameScene::LoadResources()
{
}

void NGameScene::Init()
{
#pragma region	オーディオ初期化
	NAudioManager::GetInstance()->Play("playBGM", true, 0.2f);
#pragma endregion
#pragma region	カメラ初期化
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::BeforeStart);
#pragma endregion
#pragma region 描画初期化処理
#pragma region オブジェクト生成
	NParticleManager::GetInstance()->Init();
	NCollisionManager::GetInstance()->Init();
	Player::GetInstance()->Init();
	Boss::GetInstance()->Init();
	Player::GetInstance()->SetIsElapseAnime(true);
	BulletManager::GetInstance()->Init();
	EnemyManager::GetInstance()->Init();
	ItemManager::GetInstance()->Init();
	Field::GetInstance()->Init();
	Wave::GetInstance()->Init();
#pragma endregion
#pragma region オブジェクトの初期値設定


#pragma endregion
#pragma region 背景スプライト生成
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("white");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	backSprite_->color_.SetColor255(10, 10, 10);
#pragma endregion
#pragma region 前景スプライト生成
	darken_ = std::make_unique<NSprite>();
	darken_->CreateSprite("white");
	darken_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	darken_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	darken_->color_.SetColor255(0, 0, 0, 180);

	mutekiTex_ = std::make_unique<NSprite>();
	mutekiTex_->CreateSprite("muteki");
	mutekiTex_->SetAncor({ 0.5f,0.5f });
	mutekiTex_->SetSize(1000.f, 200.f);
	mutekiTex_->rotation_ = -15.0f;
	mutekiTex_->SetPos(
		-(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	mutekiTex_->color_.SetColor255(255, 255, 255, 100);

	for (uint32_t i = 0; i < 2; i++)
	{
		movieDarken_[i] = std::make_unique<NSprite>();
		movieDarken_[i] = std::make_unique<NSprite>();
		movieDarken_[i]->CreateSprite("white");
		movieDarken_[i]->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height * 0.1f);
		if (i == 1)
		{
			movieDarken_[i]->SetPos(
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_height * i - movieDarken_[i]->GetSize().y * 0.5f);
		}
		else
		{
			movieDarken_[i]->SetPos(
				(float)NWindows::GetInstance()->kWin_width * 0.5f,
				(float)NWindows::GetInstance()->kWin_height * i + movieDarken_[i]->GetSize().y * 0.5f);
		}
		movieDarken_[i]->color_.SetColor255(0, 0, 0, 255);
	}
#pragma endregion
#pragma region 各スプライトの設定
	UIManager::GetInstance()->SetSize(UIType::Shaft, { 100.0f,150.0f });
	UIManager::GetInstance()->SetPos(UIType::Shaft, { NWindows::GetInstance()->kWin_width * 0.5f, 480.0f });
	UIManager::GetInstance()->SetSize(UIType::Lstick, { 100.0f,100.0f });
	UIManager::GetInstance()->SetPos(UIType::Lstick, { NWindows::GetInstance()->kWin_width * 0.5f, 500.0f });


	UIManager::GetInstance()->SetPos(UIType::Abutton,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f });
	UIManager::GetInstance()->SetPos(UIType::AbuttonPush,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f });

	//Aボタンはプレイ中使わないから見せない
	UIManager::GetInstance()->SetInvisible(UIType::Abutton, true);
	UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, true);

	UIManager::GetInstance()->SetPos(UIType::Clear, { -(float)NWindows::GetInstance()->kWin_width, 100.0f });
	UIManager::GetInstance()->SetSize(UIType::Clear, { 350.f, 100.f });
	UIManager::GetInstance()->SetPos(UIType::Faild, { -(float)NWindows::GetInstance()->kWin_width, 100.0f });
	UIManager::GetInstance()->SetSize(UIType::Faild, { 350.f, 100.f });

	//ポーズメニュー関連
	UIManager::GetInstance()->SetAncorPoint(UIType::Menu, { 0.f, 0.f });
	UIManager::GetInstance()->SetPos(UIType::Menu,
		{ 20.f, (float)NWindows::GetInstance()->kWin_height - 80.0f });

	UIManager::GetInstance()->SetAncorPoint(UIType::Back, { 0.f, 0.f });
	UIManager::GetInstance()->SetPos(UIType::Back,
		{ 20.f, (float)NWindows::GetInstance()->kWin_height - 80.0f });

	UIManager::GetInstance()->SetSize(UIType::Retry, { 500.f,100.f });
	UIManager::GetInstance()->SetPos(UIType::Retry,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f - 100.0f });

	UIManager::GetInstance()->SetSize(UIType::Title, { 250.f,50.f });
	UIManager::GetInstance()->SetPos(UIType::Title,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f });

	UIManager::GetInstance()->SetSize(UIType::Option, { 250.f,50.f });
	UIManager::GetInstance()->SetPos(UIType::Option,
		{ (float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f + 100.0f });

	UIManager::GetInstance()->SetUIVol();
#pragma endregion
#pragma endregion
#pragma region	ライト生成
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, true, false, false);
	lightGroup_->sPointLights[0]->SetLightAtten({ 0.2f,0.2f,0.2f });
	for (uint32_t i = 1; i <= BulletManager::GetInstance()->maxBul; i++)
	{
		lightGroup_->sPointLights[i]->SetActive(false);
	}
	lightGroup_->Update();

	/*lightGroup_->sPointLights[1]->SetActive(false);
	lightGroup_->sPointLights[2]->SetActive(false);*/
	// 3Dオブジェクトにライトをセット
	NObj3d::SetLightGroup(lightGroup_.get());

	IEmitter3D::SetLightGroup(lightGroup_.get());
#pragma endregion
#pragma region	その他設定
	Score::Init();
	Bloom::Init();

	sScene = SceneMode::BeforeStart;

	beforeStartTimer_.Start();
	stickRotTimer_.Reset();
	slidePos_ = 0.0f;
	slideTimer_.Reset();
	slideTimer_ = 0.1f;

	if (volume_.size() < 3)
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			volume_.emplace_back();
		}
	}
	volume_[0] = NAudioManager::GetInstance()->GetMasterVolume();
	volume_[1] = NAudioManager::GetInstance()->GetBGMVolume();
	volume_[2] = NAudioManager::GetInstance()->GetSEVolume();

	ItemManager::GetInstance()->Generate(NVec3(2.f, 0, 0), BulletType::MaxType, false);

	mutekiTexStartPos = {
		-(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height };
	mutekiTexEndPos = {
		(float)NWindows::GetInstance()->kWin_width + (float)NWindows::GetInstance()->kWin_width * 0.5f,
		0.f };
	mutekiTexNowPos = mutekiTexStartPos;

	mutekiInTimer_.Reset();
	mutekiOutTimer_.Reset();
#pragma endregion
}

void NGameScene::Update()
{
	if (sScene == SceneMode::Pause)		//ポーズ画面
	{
		if (isOption_ == false)
		{
			//リトライかタイトル戻るかオプションか選択
			//スティックで選択
			if (NInput::GetInstance()->StickTriggered(true) == -1)
			{
				if (pauseScene_ > (PauseSceneMode)((uint32_t)PauseSceneMode::Retry))
				{
					pauseScene_ = (PauseSceneMode)((uint32_t)pauseScene_ - 1);
				}
			}
			else if (NInput::GetInstance()->StickTriggered(true) == 1)
			{
				if (pauseScene_ < (PauseSceneMode)((uint32_t)PauseSceneMode::Option))
				{
					pauseScene_ = (PauseSceneMode)((uint32_t)pauseScene_ + 1);
				}
			}
			//キーボードで選択
			if (NInput::IsKeyDown(DIK_UP) || NInput::IsKeyDown(DIK_W))
			{
				if (pauseScene_ > (PauseSceneMode)((uint32_t)PauseSceneMode::Retry))
				{
					pauseScene_ = (PauseSceneMode)((uint32_t)pauseScene_ - 1);
				}
			}
			else if (NInput::IsKeyDown(DIK_DOWN) || NInput::IsKeyDown(DIK_S))
			{
				if (pauseScene_ < (PauseSceneMode)((uint32_t)PauseSceneMode::Option))
				{
					pauseScene_ = (PauseSceneMode)((uint32_t)pauseScene_ + 1);
				}
			}

			//「リトライ」が選ばれてる時
			if (pauseScene_ == PauseSceneMode::Retry)
			{
				UIManager::GetInstance()->SetSize(UIType::Retry, { 500.f,100.f });
				UIManager::GetInstance()->SetSize(UIType::Title, { 250.f,50.f });
				UIManager::GetInstance()->SetSize(UIType::Option, { 250.f,50.f });

				//シーン切り替え
				if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
				{
					NSceneChange::GetInstance()->Start();	//シーン遷移開始
				}
			}
			//「タイトルへ」が選ばれてる時
			else if (pauseScene_ == PauseSceneMode::Title)
			{
				UIManager::GetInstance()->SetSize(UIType::Retry, { 250.f,50.f });
				UIManager::GetInstance()->SetSize(UIType::Title, { 500.f,100.f });
				UIManager::GetInstance()->SetSize(UIType::Option, { 250.f,50.f });

				//シーン切り替え
				if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
				{
					NSceneChange::GetInstance()->Start();	//シーン遷移開始
				}
			}
			//「オプション」が選ばれてる時
			else if (pauseScene_ == PauseSceneMode::Option)
			{
				UIManager::GetInstance()->SetSize(UIType::Retry, { 250.f,50.f });
				UIManager::GetInstance()->SetSize(UIType::Title, { 250.f,50.f });
				UIManager::GetInstance()->SetSize(UIType::Option, { 500.f,100.f });

				if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
				{
					isOption_ = true;
				}
			}
		}
		else if (isOption_)
		{
			//どの音量調節するか選択
			//スティックで選択
			if (abs(NInput::GetStick().x) < 0.1f)	//横入力がある程度小さいとき
			{
				if (NInput::StickTriggered(true) == -1)
				{
					if (volumeType_ > 0)
					{
						volumeType_--;
					}
				}
				else if (NInput::StickTriggered(true) == 1)
				{
					if (volumeType_ < 2)
					{
						volumeType_++;
					}
				}
			}
			//キーボードで選択
			if (NInput::IsKeyDown(DIK_UP) || NInput::IsKeyDown(DIK_W))
			{
				if (volumeType_ > 0)
				{
					volumeType_--;
				}
			}
			else if (NInput::IsKeyDown(DIK_DOWN) || NInput::IsKeyDown(DIK_S))
			{
				if (volumeType_ < 2)
				{
					volumeType_++;
				}
			}

			//-------------------------- 音量調整 --------------------------//
			//スティックで調整
			volume_[volumeType_] += powf(NInput::GetStick().x, 2.f) * 0.01f * MathUtil::Signf(NInput::GetStick().x);

			//キーボードで調整
			if (NInput::IsKeyDown(DIK_LEFT) || NInput::IsKeyDown(DIK_A) ||
				NInput::IsKeyDown(DIK_RIGHT) || NInput::IsKeyDown(DIK_D))
			{
				volEaseTimer_.Start();	//調節キー押したらイージングタイマー開始
			}

			//押してから徐々に足される値が大きくなるため調整しやすくなってる
			if (NInput::IsKey(DIK_LEFT) || NInput::IsKey(DIK_A))
			{
				volume_[volumeType_] -= 0.01f * NEasing::InQuad(volEaseTimer_.GetTimeRate());
			}
			else if (NInput::IsKey(DIK_RIGHT) || NInput::IsKey(DIK_D))
			{
				volume_[volumeType_] += 0.01f * NEasing::InQuad(volEaseTimer_.GetTimeRate());
			}
			volEaseTimer_.Update();

			//超えないように調整
			volume_[volumeType_] = MathUtil::Clamp(volume_[volumeType_], 0.f, 1.f);

			//UIに反映
			for (uint32_t i = 0; i < 3; i++)
			{
				if (i == volumeType_)
				{
					UIManager::GetInstance()->SetUIVolPoint(i, volume_[i], 60.f);
				}
				else
				{
					UIManager::GetInstance()->SetUIVolPoint(i, volume_[i], 30.f);
				}
			}
			//音量に反映
			NAudioManager::GetInstance()->SetMasterVolume(volume_[0]);
			NAudioManager::GetInstance()->SetBGMVolume(volume_[1]);
			NAudioManager::GetInstance()->SetSEVolume(volume_[2]);
		}

		if (NInput::IsKeyDown(DIK_ESCAPE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_START))
		{
			if (isOption_)
			{
				isOption_ = false;
			}
			else
			{
				sScene = SceneMode::Play;	//プレイに戻る
			}
		}
	}
	else	//ポーズ画面以外の処理
	{
#pragma region カメラ
		NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region スプライト
		backSprite_->Update();
		darken_->Update();
		mutekiTex_->Update();
		for (uint32_t i = 0; i < 2; i++)
		{
			movieDarken_[i]->Update();
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
		if (Player::GetInstance()->GetIsGodmode())
		{
			lightGroup_->sPointLights[0]->SetLightPos(Player::GetInstance()->GetHeadPos() + NVec3(0, 0.5f, 0));
			lightGroup_->sPointLights[0]->SetLightColor(Player::GetInstance()->GetColor().GetRGB());
		}
		else
		{
			lightGroup_->sPointLights[0]->SetLightPos(ItemManager::GetInstance()->GetMutekiItemPos() + NVec3(0, 0.5f, 0));
			lightGroup_->sPointLights[0]->SetLightColor({ 0.5f,1.f,0.1f });
		}

		for (uint32_t i = 1; i <= BulletManager::GetInstance()->maxBul; i++)
		{
			lightGroup_->sPointLights[i]->SetActive(false);
		}

		for (uint32_t i = 1; i < BulletManager::GetInstance()->bullets_.size(); i++)
		{
			lightGroup_->sPointLights[i]->SetActive(true);
			lightGroup_->sPointLights[i]->SetLightPos(BulletManager::GetInstance()->bullets_[i]->GetPos());
			lightGroup_->sPointLights[i]->SetLightColor(BulletManager::GetInstance()->bullets_[i]->GetColor());
		}
		lightGroup_->Update();

		//無敵演出中なら
		if (Player::GetInstance()->GetIsMutekiDirection())
		{
			//カメラの遷移も終わったタイミングで文字出し始める
			if (mutekiInTimer_.GetStarted() == false && NCameraManager::GetInstance()->GetIsMutekiCameraChanged())
			{
				mutekiInTimer_.Start();
				NAudioManager::GetInstance()->Play("mutekiSE");	//音も鳴らす
				NAudioManager::GetInstance()->Destroy("playBGM");
			}

			//文字スライドさせて出現
			if (mutekiInTimer_.GetRun())
			{
				mutekiTexNowPos.x = NEasing::InOutBack(
					mutekiTexStartPos.x,
					(float)NWindows::GetInstance()->kWin_width * 0.5f,
					mutekiInTimer_.GetTimeRate());
				mutekiTexNowPos.y = NEasing::InOutBack(
					mutekiTexStartPos.y,
					(float)NWindows::GetInstance()->kWin_height * 0.5f,
					mutekiInTimer_.GetTimeRate());
			}
		}
		else
		{
			//演出が終わったら掃けさせる
			if (mutekiOutTimer_.GetStarted() == false && mutekiInTimer_.GetEnd())
			{
				mutekiOutTimer_.Start();
				NAudioManager::GetInstance()->Play("mutekiBGM", true, 0.2f);
			}

			//文字スライドさせて掃けさす
			if (mutekiOutTimer_.GetRun())
			{
				mutekiTexNowPos.x = NEasing::OutQuad(
					(float)NWindows::GetInstance()->kWin_width * 0.5f,
					mutekiTexEndPos.x,
					mutekiOutTimer_.GetTimeRate());
				mutekiTexNowPos.y = NEasing::OutQuad(
					(float)NWindows::GetInstance()->kWin_height * 0.5f,
					mutekiTexEndPos.y,
					mutekiOutTimer_.GetTimeRate());
			}

			//全部終わったらリセット
			if (mutekiOutTimer_.GetEnd())
			{
				mutekiInTimer_.Reset();
				mutekiOutTimer_.Reset();
			}
		}

		mutekiTex_->SetPos(mutekiTexNowPos.x, mutekiTexNowPos.y);
		mutekiInTimer_.Update();
		mutekiOutTimer_.Update();

		if (sScene == SceneMode::BeforeStart)	//始まる前の処理
		{
			beforeStartTimer_.Update();

			if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
			{
				beforeStartTimer_.SetEnd(true);
			}

			//演出終わったらプレイシーンに移行
			if (beforeStartTimer_.GetEnd())
			{
				NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
				sScene = SceneMode::Play;
			}
		}
		else if (sScene == SceneMode::Play)	//プレイ中の処理
		{
			if (NInput::IsKeyDown(DIK_ESCAPE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_START))
			{
				sScene = SceneMode::Pause;	//ポーズ画面に切り替え
			}

			Player::GetInstance()->Update();
			Boss::GetInstance()->Update();

			if (Field::GetInstance()->GetIsStart())
			{
				//スタート線スライドタイマー開始
				if (slideTimer_.GetStarted() == false)
				{
					slideTimer_.Start();
				}
				slideTimer_.Update();

				slidePos_ = NEasing::InQuad(0.0f, -(float)NWindows::GetInstance()->kWin_width, slideTimer_.GetTimeRate());
				UIManager::GetInstance()->SetPos(UIType::Shaft,
					{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 500.0f });
			}

			stickRotTimer_.Roop();
			NVec2 stickVec = { sinf(stickRotTimer_.GetTimeRate() * PI2),cosf(stickRotTimer_.GetTimeRate() * PI2) };
			stickVec *= 10.f;
			//Yはスティックだと上が正の値なので引く
			UIManager::GetInstance()->SetPos(UIType::Lstick,
				{ NWindows::GetInstance()->kWin_width * 0.5f + stickVec.x + slidePos_, 500.0f - stickVec.y });

			//プレイヤーが無敵じゃないとき
			if (Player::GetInstance()->GetIsGodmode() == false)
			{
				//プレイヤーが波に飲み込まれたら殺す
				if (Wave::GetInstance()->GetFrontPosZ() > Player::GetInstance()->GetFrontPosZ())
				{
					if (Player::GetInstance()->GetIsAlive())	//生きてるなら殺す
					{
						Player::GetInstance()->SetIsAlive(false);
					}
				}
			}

			//プレイヤーが死んで、死亡演出が終わったら失敗リザルトへ
			if (Player::GetInstance()->GetIsAlive() == false &&
				Player::GetInstance()->GetDeadEffectEnd())
			{
				NAudioManager::GetInstance()->Destroy("mutekiBGM");
				NAudioManager::GetInstance()->Destroy("playBGM");
				NAudioManager::GetInstance()->Play("faildBGM", true, 0.2f);
				sScene = SceneMode::Faild;
				slideTimer_.Reset();
				slideTimer_ = 0.5f;
				Player::GetInstance()->FaildUpdate();	//ここでプレイヤーの座標変えてあげないとカメラの座標が死んだ座標基準になっちゃう
				Player::GetInstance()->SetIsElapseAnime(false);
				Score::SaveScore();	//死亡演出終わるまではスコア入れたげる
				NCameraManager::GetInstance()->ChangeCameara(CameraType::Faild);
			}

			//ゴールしたらクリアリザルトへ
			if (Field::GetInstance()->GetIsGoal())
			{
				NAudioManager::GetInstance()->Destroy("playBGM");
				NAudioManager::GetInstance()->Destroy("mutekiBGM");
				NAudioManager::GetInstance()->Play("clearBGM", true, 0.2f);
				sScene = SceneMode::Clear;
				slideTimer_.Reset();
				slideTimer_ = 0.5f;
				Player::GetInstance()->SetIsElapseAnime(false);
				Score::SaveScore();	//死亡演出終わるまではスコア入れたげる
				NCameraManager::GetInstance()->ChangeCameara(CameraType::Clear);
			}

			//当たり判定総当たり
			NCollisionManager::GetInstance()->CheckAllCollision();
		}
		else if (sScene == SceneMode::Clear)	//クリアリザルトの処理
		{
			Player::GetInstance()->ClearUpdate();

			//スタート線スライドタイマー開始
			if (slideTimer_.GetStarted() == false)
			{
				slideTimer_.Start();
			}
			slideTimer_.Update();
			//クリアテキストスライド
			slidePos_ = NEasing::InOutBack(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
			UIManager::GetInstance()->SetPos(UIType::Clear,
				{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f });

			//リザルトスコアが上から落ちてくる
			float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer_.GetTimeRate());
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
				UIManager::GetInstance()->SetInvisible(UIType::Abutton, true);
				UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, false);
			}
			else
			{
				UIManager::GetInstance()->SetInvisible(UIType::Abutton, false);
				UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, true);
			}

			//シーン切り替え
			if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
			{
				NSceneChange::GetInstance()->Start();	//シーン遷移開始
			}
		}
		else if (sScene == SceneMode::Faild)	//失敗リザルトの処理
		{
			lightGroup_->sPointLights[0]->SetActive(false);
			Player::GetInstance()->FaildUpdate();

			//スタート線スライドタイマー開始
			if (slideTimer_.GetStarted() == false)
			{
				slideTimer_.Start();
			}
			slideTimer_.Update();
			//失敗テキストスライド
			slidePos_ = NEasing::InQuad(-(float)NWindows::GetInstance()->kWin_width, 0.0f, slideTimer_.GetTimeRate());
			UIManager::GetInstance()->SetPos(UIType::Faild,
				{ NWindows::GetInstance()->kWin_width * 0.5f + slidePos_, 100.0f });

			//リザルトスコアが上から落ちてくる
			float slideP = NEasing::InOutBack(-Score::GetSize(Score::TexType::Result).y, 300.0f, slideTimer_.GetTimeRate());
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
				UIManager::GetInstance()->SetInvisible(UIType::Abutton, true);
				UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, false);
			}
			else
			{
				UIManager::GetInstance()->SetInvisible(UIType::Abutton, false);
				UIManager::GetInstance()->SetInvisible(UIType::AbuttonPush, true);
			}

			//シーン切り替え
			if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
			{
				NSceneChange::GetInstance()->Start();	//シーン遷移開始
			}
		}
	}

	//切り替えてﾖｼって言われたら
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NAudioManager::GetInstance()->Destroy("mutekiBGM");
		NAudioManager::GetInstance()->Destroy("playBGM");
		NAudioManager::GetInstance()->Destroy("clearBGM");
		NAudioManager::GetInstance()->Destroy("faildBGM");
		//「リトライ」が選ばれてる時
		if (pauseScene_ == PauseSceneMode::Retry)
		{
			NSceneManager::ChangeScene<NGameScene>();		//ゲームシーンに切り替え
		}
		else
		{
			NSceneManager::ChangeScene<NTitleScene>();		//タイトルシーンに切り替え
		}
		NSceneChange::GetInstance()->SetIsChange(false);	//切り替えちゃﾀﾞﾒｰ
	}
#ifdef _DEBUG
	//シーン切り替え(デバッグ用)
	if (NInput::IsKeyDown(DIK_RETURN) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_X))
	{
		NSceneChange::GetInstance()->Start();	//シーン遷移開始
	}

	//アイテム出すボタン(デバッグ用)
	if (NInput::IsKeyDown(DIK_I))
	{
		ItemManager::GetInstance()->Generate(NVec3::zero, BulletType::LineBullet);
	}
#endif
	}

void NGameScene::DrawBackSprite()
{
	backSprite_->Draw();
}

void NGameScene::DrawBack3D()
{
	Field::GetInstance()->Draw();
	if (sScene == SceneMode::Play || sScene == SceneMode::Pause)	//プレイ中かポーズ中
	{
		Wave::GetInstance()->DrawObj();
	}
}

void NGameScene::Draw3D()
{
	if (sScene == SceneMode::Play || sScene == SceneMode::Pause)	//プレイ中かポーズ中
	{
		BulletManager::GetInstance()->Draw();
		EnemyManager::GetInstance()->Draw();
		Boss::GetInstance()->Draw();
		ItemManager::GetInstance()->Draw();
	}
	else if (sScene == SceneMode::Faild)	//失敗シーン
	{
		EnemyManager::GetInstance()->Draw();
		Boss::GetInstance()->Draw();
		ItemManager::GetInstance()->Draw();
	}
	Player::GetInstance()->Draw();
}

void NGameScene::DrawParticle()
{
	NParticleManager::GetInstance()->Draw();
}

void NGameScene::DrawForeSprite()
{
	if (sScene == SceneMode::Pause)	//ポーズ画面
	{
		darken_->Draw();

		if (isOption_)
		{
			UIManager::GetInstance()->DrawUIVol();
		}
		else if (isOption_ == false)
		{
			UIManager::GetInstance()->Draw(UIType::Retry);
			UIManager::GetInstance()->Draw(UIType::Title);
			UIManager::GetInstance()->Draw(UIType::Option);
		}

		UIManager::GetInstance()->Draw(UIType::Back);
	}
	else
	{
		if (Player::GetInstance()->GetIsMutekiDirection() || Boss::GetInstance()->GetIsEntry())
		{
			for (uint32_t i = 0; i < 2; i++)
			{
				movieDarken_[i]->Draw();
			}
		}

		mutekiTex_->Draw();

		UIManager::GetInstance()->Draw(UIType::Menu);

		if (Player::GetInstance()->GetIsMutekiDirection() == false)
		{
			UIManager::GetInstance()->Draw(UIType::Shaft);
			UIManager::GetInstance()->Draw(UIType::Lstick);
		}
	}

	if (sScene == SceneMode::Play)	//プレイ中
	{
		Wave::GetInstance()->DrawSprite();
	}

	UIManager::GetInstance()->Draw(UIType::Abutton);
	UIManager::GetInstance()->Draw(UIType::AbuttonPush);
	UIManager::GetInstance()->Draw(UIType::Clear);
	UIManager::GetInstance()->Draw(UIType::Faild);
	UIManager::GetInstance()->DrawUIBul();

	Score::Draw();
}