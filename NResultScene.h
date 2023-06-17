#pragma once
#include <vector>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NAudio.h"
#include "NCamera.h"
#include "NCollision.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NResultScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material_;				//�}�e���A��
	static const uint32_t kMaxObj = 3;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj_;			//�I�u�W�F�N�g(�萔�o�b�t�@)

	NCollision::Sphere sphere_;
	NCollision::Plane plane_;
	bool isCol_ = false;

	//�w�i�X�v���C�g
	static const uint32_t kMaxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite_[kMaxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const uint32_t kMaxForeSprite = 3;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> foreSprite_[kMaxForeSprite];	//�O�i�X�v���C�g
	
	NCamera camera_;	//�J����

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

public:
	//�C���X�^���X�擾
	static NResultScene* GetInstance();

	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();
};