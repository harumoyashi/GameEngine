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
#include "NDirectionalLight.h"
#include "NPointLight.h"
#include "NSpotLight.h"
#include "NCircleShadow.h"
#include "NTimer.h"
#include "AssimpLoader.h"
#include "NConstBuff.h"
#include "LevelDataLoader.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class NTitleScene
{
private:
	//�I�u�W�F�N�g
	NMaterial material;				//�}�e���A��
	static const uint32_t maxObj = 3;	//�I�u�W�F�N�g��
	std::vector<std::unique_ptr<NObj3d>> obj;	//�I�u�W�F�N�g
	std::vector<std::unique_ptr<NObj3d>> levelDataobj;	//���x���f�[�^����ǂݍ��񂾃I�u�W�F�N�g

	static const uint32_t maxModel = 2;	//���f����
	std::vector<NModel> model;	//���f�����

	NCollision::Sphere sphere;
	NCollision::Plane plane;
	bool isCol = false;

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

	NTimer timer;

	bool flag = true;
	float color = 0.0f;

	std::vector<Mesh> meshes; // ���b�V���̔z��
	std::vector<NVertexBuff*> vertexBuffers; // ���b�V���̐����̒��_�o�b�t�@
	std::vector<NIndexBuff*> indexBuffers; // ���b�V���̐����̃C���f�b�N�X�o�b�t�@
	const wchar_t* modelFile = L"Resources/FBX/Alicia_solid_Unity.FBX";

	ImportSettings importSetting = // ���ꎩ�͎̂���̓ǂݍ��ݐݒ�\����
	{
		modelFile,
		meshes,
		false,
		true // �A���V�A�̃��f���́A�e�N�X�`����UV��V�������]���Ă���ۂ��H�̂œǂݍ��ݎ���UV���W���t�]������
	};
	
	AssimpLoader loader;
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans;

	std::unique_ptr<LevelData> levelData;

public:
	//�C���X�^���X�擾
	static NTitleScene* GetInstance();

	NTitleScene();
	~NTitleScene();

	void Init();
	void Update();
	void Draw();
	void Reset();
	void Finalize();

	//���x���G�f�B�^����o�͂��ꂽ�ǂݍ��ݍσI�u�W�F�N�g��z�u���Ă���
	void SetObject(LevelData* levelData);
};