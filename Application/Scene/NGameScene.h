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
#include "NAudio.h"

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

	SceneMode scene = SceneMode::Play;

	//�I�u�W�F�N�g


	//�w�i�X�v���C�g
	

	//�O�i�X�v���C�g


	//�I�[�f�B�I
	NAudio* audio_ = nullptr;

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	void LoadResources();
	void Init() override;
	void Update() override;
	//�w�i3D�I�u�W�F�N�g
	void DrawBack3D()override;
	//�w�i�X�v���C�g
	void DrawBackSprite()override;
	//3D�I�u�W�F�N�g
	void Draw3D() override;
	//�p�[�e�B�N��
	void DrawParticle() override;
	//�O�i�X�v���C�g
	void DrawForeSprite() override;

};