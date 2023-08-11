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
	NCameraManager::GetInstance()->ChangeCameara(CameraType::Normal);
#pragma endregion
#pragma region �`�揉��������
	//�I�u�W�F�N�g

#pragma region �I�u�W�F�N�g�̏����l�ݒ�

#pragma endregion
	//FBX�ǂݍ��ݖY��Ȃ��p
	/*assimpModel_.Load("boneTest");
	assimpModel_.Init();
	assimpModel_.position_ = { 0,0,1 };
	assimpModel_.rotation_ = NVec3::zero;
	assimpModel_.scale_ = { 0.03f,0.03f,0.03f };*/

	//�w�i�X�v���C�g����

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
#pragma region �J����
	NCameraManager::GetInstance()->Update();
#pragma endregion

	//assimpModel_.Update();

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
}

void NTestScene::DrawBack3D()
{
}

void NTestScene::Draw3D()
{
	//assimp���f���`��//
	//assimpModel_.Draw();
}

void NTestScene::DrawParticle()
{
}

void NTestScene::DrawForeSprite()
{
}