#pragma once
#include <vector>
#include <wrl.h>

#include "IScene.h"
#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NCollision.h"
#include "NLightGroup.h"
#include "NEasing.h"
#include "NConstBuff.h"
#include "NLevelDataLoader.h"
#include "NAssimpModel.h"

class NTestScene final :
	public IScene
{
private:
	//�I�u�W�F�N�g
	static const uint32_t kMaxObj = 3;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj_;			//�I�u�W�F�N�g(�萔�o�b�t�@)
	std::vector<std::unique_ptr<NObj3d>> levelDataobj_;	//���x���f�[�^����ǂݍ��񂾃I�u�W�F�N�g

	bool isCol_ = false;

	//�w�i�X�v���C�g
	static const uint32_t kMaxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const uint32_t kMaxForeSprite = 3;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//�O�i�X�v���C�g

	//NAssimpModel assimpModel_;

	std::unique_ptr<LevelData> levelData_;

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	NTestScene();
	~NTestScene();

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