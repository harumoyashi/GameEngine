#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "NRootParam.h"
#include "NMaterial.h"
#include "NGPipeline.h"
#include "NRootSignature.h"
#include "NObj3d.h"
#include "NTexture.h"
#include "NSprite.h"
#include "NPreDraw.h"
#include "NModel.h"
#include "NCamera.h"
#include "NAudio.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material_;				//�}�e���A��
	static const uint32_t kMaxObj = 4;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj_;			//�I�u�W�F�N�g(�萔�o�b�t�@)

	static const uint32_t kMaxModel = 3;	//���f����
	std::vector<NModel> model_;	//���f�����

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

	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw();
	void Reset();
	//�I������
	void Finalize();

private:


};