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
#include "NLightGroup.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NGameScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const int maxObj = 4;	//�I�u�W�F�N�g��
	std::unique_ptr<NObj3d> obj[maxObj];			//�I�u�W�F�N�g(�萔�o�b�t�@)

	static const int maxModel = 3;	//���f����
	std::array<std::unique_ptr<NModel>, maxModel> model;	//���f�����

	//�w�i�X�v���C�g
	static const int maxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite[maxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const int maxForeSprite = 3;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> foreSprite[maxForeSprite];	//�O�i�X�v���C�g

	NCamera camera;	//�J����

	//�I�[�f�B�I
	NAudio* audio = nullptr;
	static const int maxSoundData = 3;		//�T�E���h�f�[�^�̍ő吔
	uint32_t soundData[maxSoundData] = {};	//�T�E���h�f�[�^�i�[�p

	std::unique_ptr<NLightGroup> lightGroup;

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