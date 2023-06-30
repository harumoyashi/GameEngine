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
#include "NCamera.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene:
	public IScene
{
private:
	//�I�u�W�F�N�g

	//�w�i�X�v���C�g
	static const uint32_t kMaxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const uint32_t kMaxForeSprite = 3;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//�O�i�X�v���C�g

	NCamera camera_;	//�J����

	//�I�[�f�B�I
	NAudio* audio_ = nullptr;

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

public:


public:
	//�C���X�^���X�擾
	static NGameScene* GetInstance();

	void LoadResources();
	void Init() override;
	void Update() override;
	//�w�i�X�v���C�g
	void DrawBackSprite()override;
	//3D�I�u�W�F�N�g
	void Draw3D() override;
	//�O�i�X�v���C�g
	void DrawForeSprite() override;

private:


};