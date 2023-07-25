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
#include "NAssimpModel.h"
#include "NConstBuff.h"
#include "NLevelDataLoader.h"

class NTitleScene final :
	public IScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material_;				//�}�e���A��
	static const uint32_t kMaxObj = 3;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj_;	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<NObj3d>> levelDataobj_;	//���x���f�[�^����ǂݍ��񂾃I�u�W�F�N�g

	Sphere sphere_;
	Plane plane_;
	bool isCol_ = false;

	//�w�i�X�v���C�g
	static const uint32_t kMaxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	std::unique_ptr<NSprite> titleLogo_;						//�^�C�g�����S
	std::vector<std::unique_ptr<NSprite>> aButton_{ 2 };		//A�{�^��UI
	NEasing::EaseTimer flashingTimer_ = 1.0f;					//�_�Ń^�C�}�[

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

	std::unique_ptr<LevelData> levelData_;

	NAssimpModel assimpModel_;

public:
	//�C���X�^���X�擾
	static NTitleScene* GetInstance();

	NTitleScene();
	~NTitleScene();

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