#include "Wave.h"
#include "Field.h"
#include "Player.h"
#include "NGameScene.h"

#include "NImGuiManager.h"
#include "imgui.h"

Wave* Wave::GetInstance()
{
	static Wave instance;
	return &instance;
}

void Wave::Init()
{
	scaleZ_ = 20.0f;
	posZ_ = Field::GetInstance()->GetStartPos() - 15.0f - scaleZ_;
	moveSpeed_ = 0.02f;		//プレイヤーが0.05fの場合
#pragma region オブジェクトの生成,設定
	for (uint32_t i = 0; i < waveDivide_; i++)
	{
		obj_.emplace_back();
		obj_.back() = std::make_unique<NObj3d>();
		obj_.back()->Init();

		obj_.back()->SetModel("plane");
		//フィールドの横幅をもとに分割する
		float waveScaleX = Field::GetInstance()->GetActivityAreaX() * 2.0f;
		obj_.back()->scale_ =
		{ waveScaleX / waveDivide_,1.0f, scaleZ_ };
		obj_.back()->position_ =
		{ obj_.back()->scale_.x * 2.0f * i - obj_.back()->scale_.x - waveScaleX, 0,  posZ_ };
		obj_.back()->color_.SetColor255(5, 5, 255);

		randomPosZ_.emplace_back();
		randomPosZ_.back() = MathUtil::Randomf(0.5f, 1.5f);

		waveTimer_.emplace_back();
		waveTimer_.back() = MathUtil::Randomf(0.5f, 2.0f);
	}
#pragma endregion
#pragma region スプライトの生成,設定
	waveUI_ = std::make_unique<NSprite>();
	waveUI_->CreateSprite("waveUI", { 0.5f,0.5f });
	waveUI_->SetSize(100.f, 100.f);
	waveUI_->SetPos(NWindows::kWin_width * 0.5f, 650.f);
	waveUI_->color_ = obj_[0]->color_;

	meterUI_ = std::make_unique<NSprite>();
	meterUI_->CreateSprite("meter");
	meterUI_->SetPos(NWindows::kWin_width * 0.5f + 30.f, 582.f);
	meterUI_->SetSize(20.f,15.f);

	player2WaveLen_ = 0;
	meterTex_.Create(3, 0.3f);
	meterTex_.SetSize({ 20.f,20.f });
	meterTex_.SetPos({ NWindows::kWin_width * 0.5f - 40.f, 570.f });
#pragma endregion
}

void Wave::Update()
{
	//プレイ中かつスタートしてたら動くように
	if (NGameScene::GetIsPlay() == false || Field::GetInstance()->GetIsStart() == false)
	{
		moveSpeed_ = 0.0f;
	}
	else
	{
		static float moveS = 0.02f;
#ifdef _DEBUG
		ImGui::Begin("WaveParameter");
		ImGui::SliderFloat("MoveSpeed", &moveS, 0.00f, 0.5f);
		ImGui::End();
#endif //DEBUG
		moveSpeed_ = moveS;
	}

	for (size_t i = 0; i < obj_.size(); i++)
	{
		float waveScaleX = Field::GetInstance()->GetActivityAreaX() * 2.0f;
		obj_[i]->scale_ =
		{ waveScaleX / waveDivide_,1.0f, scaleZ_ };
		obj_[i]->position_.x = obj_[i]->scale_.x * 2.0f * i - obj_[i]->scale_.x - waveScaleX;
	}

	posZ_ += moveSpeed_;

	for (size_t i = 0; i < obj_.size(); i++)
	{
		waveTimer_[i].RoopReverse();
		float plusPos = sinf(randomPosZ_[i] * waveTimer_[i].GetTimeRate());
		obj_[i]->position_.z = posZ_ + plusPos;

		obj_[i]->Update();
	}

	waveUI_->Update();
	meterUI_->Update();

	//プレイヤーと波の距離取る
	player2WaveLen_ = (uint32_t)(Player::GetInstance()->GetFrontPosZ() - GetFrontPosZ());
	player2WaveLen_ = MathUtil::Clamp(player2WaveLen_,0U,999U);	//マイナスにならないように
	meterTex_.SetNum(player2WaveLen_);
	meterTex_.Update();
}

void Wave::DrawObj()
{
	for (auto& obj : obj_)
	{
		obj->SetBlendMode(BlendMode::Alpha);
		obj->Draw();
		obj->SetBlendMode(BlendMode::None);
	}
}

void Wave::DrawSprite()
{
	//波が追っかけてくる時だけ表示
	if (NGameScene::GetIsPlay() && Field::GetInstance()->GetIsStart())
	{
		waveUI_->Draw();
		meterUI_->Draw();
		meterTex_.Draw();
	}
}