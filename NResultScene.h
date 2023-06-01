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
#include "NModel.h"
#include "NAudio.h"
#include "NCamera.h"
#include "NCollision.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NResultScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const int maxObj = 3;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj;			//�I�u�W�F�N�g(�萔�o�b�t�@)

	static const int maxModel = 2;	//���f����
	std::vector<NModel> model;	//���f�����

	NCollision::Sphere sphere;
	NCollision::Plane plane;
	bool isCol = false;

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

	//���C�g����
	std::unique_ptr<NDirectionalLight> directionalLight;
	std::unique_ptr<NPointLight > pointLight;
	std::unique_ptr<NSpotLight> spotLight;
	std::unique_ptr<NCircleShadow> circleShadow;

public:
	//�C���X�^���X�擾
	static NResultScene* GetInstance();

	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();
};