#include "Wave.h"
#include "Field.h"

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
		obj_.back()->scale_ = 
		{ Field::GetInstance()->GetScale().x / waveDivide_,1.0f, scaleZ_ };
		obj_.back()->position_ =
		{ obj_.back()->scale_.x * i - obj_.back()->scale_.x * 0.5f, 0,  posZ_ };
		obj_.back()->color_.SetColor255(255, 5, 5);
	}
#pragma endregion
}

void Wave::Update()
{
	//スタートしてたら動くように
	if (Field::GetInstance()->GetIsStart() == false)
	{
		moveSpeed_ = 0.0f;
	}
	else
	{
		//リリースでもいじりたいからifdefで囲ってない
		static float moveS = 0.02f;
		ImGui::Begin("WaveParameter");
		ImGui::SliderFloat("MoveSpeed", &moveS, 0.02f, 0.5f);
		ImGui::End();
		moveSpeed_ = moveS;
	}

	posZ_ += moveSpeed_;

	for (auto& obj : obj_)
	{
		obj->position_.z = posZ_ /*+ 各々の動き*/;

		obj->Update();
	}
}

void Wave::Draw()
{
	for (auto& obj : obj_)
	{
		obj->Draw();
	}
}