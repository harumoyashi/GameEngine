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
	moveSpeed_ = 0.02f;		//�v���C���[��0.05f�̏ꍇ
#pragma region �I�u�W�F�N�g�̐���,�ݒ�
	for (uint32_t i = 0; i < waveDivide_; i++)
	{
		obj_.emplace_back();
		obj_.back() = std::make_unique<NObj3d>();
		obj_.back()->Init();

		obj_.back()->SetModel("plane");
		//�t�B�[���h�̉��������Ƃɕ�������
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
	//�X�^�[�g���Ă��瓮���悤��
	if (Field::GetInstance()->GetIsStart() == false)
	{
		moveSpeed_ = 0.0f;
	}
	else
	{
		//�����[�X�ł������肽������ifdef�ň͂��ĂȂ�
		static float moveS = 0.02f;
		ImGui::Begin("WaveParameter");
		ImGui::SliderFloat("MoveSpeed", &moveS, 0.02f, 0.5f);
		ImGui::End();
		moveSpeed_ = moveS;
	}

	posZ_ += moveSpeed_;

	for (auto& obj : obj_)
	{
		obj->position_.z = posZ_ /*+ �e�X�̓���*/;

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