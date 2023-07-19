#include "NDX12.h"
#include "NTitleScene.h"
#include "NGameScene.h"
#include "NSceneManager.h"
#include "NSceneChange.h"
#include "NAudioManager.h"
#include "NModelManager.h"
#include "NInput.h"
#include "NQuaternion.h"
#include "NMathUtil.h"
#include "NCameraManager.h"
#include "NPostEffect.h"

#include "Player.h"
#include "Field.h"

NTitleScene* NTitleScene::GetInstance()
{
	static NTitleScene instance;
	return &instance;
}

NTitleScene::NTitleScene()
{
}

NTitleScene::~NTitleScene()
{
}

void NTitleScene::LoadResources()
{
}

void NTitleScene::Init()
{
#pragma region	�I�[�f�B�I������
	NAudio::GetInstance()->Init();
	NAudioManager::Play("titleBGM", true, 0.2f);
#pragma endregion
#pragma region	�J����������
	NCameraManager::GetInstance()->Init();
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Title);
#pragma endregion
#pragma region �`�揉��������

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion

	//assimpModel_.Load(L"Resources/FBX/Alicia_solid_Unity.FBX");
	//assimpModel_.Load(L"Resources/Tripping.fbx");
	/*assimpModel_.Load(L"Resources/Cat_fixed.fbx");
	assimpModel_.Init();
	assimpModel_.position_ = {0,0,1};
	assimpModel_.rotation_ = {0,0,0};
	assimpModel_.scale_ = {0.03f,0.03f,0.03f};*/

	Player::GetInstance()->Init();
	//Player::GetInstance()->SetPos(NVector3(0,0,0));

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion
	//�w�i�X�v���C�g����

	//�O�i�X�v���C�g����
	titleLogo_ = std::make_unique<NSprite>();
	titleLogo_->CreateSprite("logo");
	titleLogo_->SetPos(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	titleLogo_->SetSize(
		(float)NWindows::GetInstance()->kWin_width * 0.5f,
		(float)NWindows::GetInstance()->kWin_height * 0.5f);
	titleLogo_->color_.SetColor255(255, 255, 255, 255);

	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i] = std::make_unique<NSprite>();
		aButton_[i]->CreateClipSprite("Abutton", { i * 192.f,0 }, { 192.f,192.f });
		aButton_[i]->SetPos(
			(float)NWindows::GetInstance()->kWin_width * 0.5f, 600.f);
	}

#pragma endregion
	// ���C�g����
	lightGroup_ = std::make_unique<NLightGroup>();
	lightGroup_->Init(true, false, false, false);
	//lightGroup_->SetSpotLightColor({0,0,1});
	lightGroup_->TransferConstBuffer();
	// 3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	NObj3d::SetLightGroup(lightGroup_.get());
	NAssimpModel::SetLightGroup(lightGroup_.get());

	NPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g����
}

void NTitleScene::Update()
{
	//���C�g�����̍X�V
	lightGroup_->Update();

#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion
	titleLogo_->Update();
	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i]->Update();
	}

	flashingTimer_.Update();
	if (flashingTimer_.GetStarted() == false)
	{
		flashingTimer_.Start();
	}
	else if (flashingTimer_.GetEnd())
	{
		flashingTimer_.Reset();
	}

	if (flashingTimer_.GetTimeRate() > 0.7f)
	{
		aButton_[0]->isInvisible_ = true;
		aButton_[1]->isInvisible_ = false;
	}
	else
	{
		aButton_[0]->isInvisible_ = false;
		aButton_[1]->isInvisible_ = true;
	}

	//assimpModel_.Update();

	Player::GetInstance()->SetIsMove(false);
	Player::GetInstance()->Update();

	//�V�[���؂�ւ�
	if (NInput::IsKeyDown(DIK_SPACE) || NInput::GetInstance()->IsButtonDown(XINPUT_GAMEPAD_A))
	{
		NSceneChange::GetInstance()->Start();	//�V�[���J�ڊJ�n
	}

	//�؂�ւ���ּ���Č���ꂽ��
	if (NSceneChange::GetInstance()->GetIsChange() == true)
	{
		NSceneManager::ChangeScene<NGameScene>();			//�^�C�g���V�[���ɐ؂�ւ�
		NSceneChange::GetInstance()->SetIsChange(false);	//�؂�ւ�������Ұ
	}
}

void NTitleScene::DrawBack3D()
{
}

void NTitleScene::DrawBackSprite()
{
}

void NTitleScene::Draw3D()
{
	//assimp���f���`��//
	//assimpModel_.Draw();
	Player::GetInstance()->Draw();
}

void NTitleScene::DrawParticle()
{
}

void NTitleScene::DrawForeSprite()
{
	/*titleLogo_->Draw();
	for (uint32_t i = 0; i < aButton_.size(); i++)
	{
		aButton_[i]->Draw();
	}*/
}