#include "NDX12.h"
#include "NTestScene.h"
#include "NSceneManager.h"
#include "NTitleScene.h"
#include "NCameraManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "NPostEffect.h"
#include "RadialBlur.h"
#include "GaussianBlur.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

NTestScene::NTestScene()
{
}

NTestScene::~NTestScene()
{
}

void NTestScene::LoadResources()
{
}

void NTestScene::Init()
{
#pragma region	�I�[�f�B�I������

#pragma endregion
#pragma region	�J����������
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Debug);
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("catWalk");
	obj_->Init();
#pragma region �I�u�W�F�N�g�̏����l�ݒ�
	obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�
	obj_->SetIsElapseAnime(false);	//�o�ߎ��Ԗ������Ă���
	obj_->Update();
#pragma endregion
	//�w�i�X�v���C�g����
	backSprite_ = std::make_unique<NSprite>();
	backSprite_->CreateSprite("logo");
	backSprite_->SetSize((float)NWindows::GetInstance()->kWin_width, (float)NWindows::GetInstance()->kWin_height);
	backSprite_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	//backSprite_->color_.SetColor255(50, 50, 50);

	//�O�i�X�v���C�g����

#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
	//NAssimpModel::SetLightGroup(lightGroup_.get());
}

void NTestScene::Update()
{
	//ImGui::ShowDemoWindow();

#ifdef _DEBUG
	ImGui::Begin("PostEffectType");
	static int postEffectNum;
	const char* items[] = { "NoEffect","GaussianBlur","RadianBlur","CG4" };
	if (ImGui::Combo("PostEffect Choice", &postEffectNum, items, IM_ARRAYSIZE(items)))
	{
		switch (postEffectNum)
		{
		case 0:
			NPostEffect::SetIsActive(false);

			break;

		case 1:
			GaussianBlur::Init();

			break;

		case 2:
			RadialBlur::Init();

			break;

		case 3:
			NPostEffect::Init();

			break;

		default:
			break;
		}
	}

	ImGui::End();
#endif _DEBUG //�|�X�g�G�t�F�N�gImGui

#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
#pragma region �X�v���C�g
	backSprite_->Update();
#pragma endregion
	obj_->Update();

	//���C�g�����̍X�V
	lightGroup_->Update();

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
	}

	//�؂�ւ���ּ���Č���ꂽ��
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NTitleScene>();			//�^�C�g���V�[���ɐ؂�ւ�
		NSceneChange::GetInstance()->SetIsChange(false);	//�؂�ւ�������Ұ
	}
}

void NTestScene::DrawBackSprite()
{
	backSprite_->Draw();
}

void NTestScene::DrawBack3D()
{
}

void NTestScene::Draw3D()
{
	obj_->SetBlendMode(BlendMode::None);
	obj_->Draw();
	obj_->SetBlendMode(BlendMode::None);
}

void NTestScene::DrawParticle()
{
}

void NTestScene::DrawForeSprite()
{
}