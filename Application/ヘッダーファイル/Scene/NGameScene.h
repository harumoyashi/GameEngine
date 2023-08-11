#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "IScene.h"
#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NModel.h"

#include "SimpleParticle.h"

class NGameScene final :
	public IScene
{
private:
	//�Q�[���V�[�����ł̃V�[������
	enum class SceneMode
	{
		Play,	//�v���C��
		Clear,	//�N���A���U���g
		Faild,	//���s���U���g
	};

	//�O�i�X�v���C�g�̎��
	enum class FSpriteType
	{
		Shaft,
		LStick,
		Abutton,
		AbuttonPush,
		Clear,
		Faild,

		MaxForeSprite
	};

	SceneMode scene = SceneMode::Play;

	//�I�u�W�F�N�g


	//�w�i�X�v���C�g
	std::unique_ptr<NSprite> backSprite_;	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	std::vector<std::unique_ptr<NSprite>> foreSprite_{ (uint32_t)FSpriteType::MaxForeSprite };		//L�X�e�B�b�NUI
	float slidePos = 0.0f;							//���ɑ|���Ă����̃X���C�h�ʒu
	NEasing::EaseTimer slideTimer = 0.1f;			//�X���C�h���̃C�[�W���O�p
	NEasing::EaseTimer flashingTimer_ = 1.0f;		//�_�Ń^�C�}�[

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	void LoadResources();
	void Init() override;
	void Update() override;
	//�w�i�X�v���C�g
	void DrawBackSprite()override;
	//�w�i3D�I�u�W�F�N�g
	void DrawBack3D()override;
	//3D�I�u�W�F�N�g
	void Draw3D() override;
	//�p�[�e�B�N��
	void DrawParticle() override;
	//�O�i�X�v���C�g
	void DrawForeSprite() override;

};