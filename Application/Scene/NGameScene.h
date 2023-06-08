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
	static const uint32_t maxObj = 4;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj;			//�I�u�W�F�N�g(�萔�o�b�t�@)

	static const uint32_t maxModel = 3;	//���f����
	std::vector<NModel> model;	//���f�����

	//�w�i�X�v���C�g
	static const uint32_t maxBackSprite = 3;	//�w�i�X�v���C�g��
	std::unique_ptr<NSprite> backSprite[maxBackSprite];	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	static const uint32_t maxForeSprite = 3;	//�O�i�X�v���C�g��
	std::unique_ptr<NSprite> foreSprite[maxForeSprite];	//�O�i�X�v���C�g

	NCamera camera;	//�J����

	//�I�[�f�B�I
	NAudio* audio = nullptr;
	static const uint32_t maxSoundData = 3;		//�T�E���h�f�[�^�̍ő吔
	uint32_t soundData[maxSoundData] = {};	//�T�E���h�f�[�^�i�[�p

	//���C�g����
	std::unique_ptr<NDirectionalLight> directionalLight;
	std::unique_ptr<NPointLight > pointLight;
	std::unique_ptr<NSpotLight> spotLight;
	std::unique_ptr<NCircleShadow> circleShadow;

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