#pragma once
#include <vector>
#include <algorithm>
#include <wrl.h>

#include "IScene.h"
#include "NObj3d.h"
#include "NSprite.h"
#include "SimpleParticle.h"
#include "NEasing.h"

//�Q�[���V�[�����ł̃V�[������
enum class SceneMode
{
	Play,	//�v���C��
	Clear,	//�N���A���U���g
	Faild,	//���s���U���g
};

class NGameScene final :
	public IScene
{
private:
	//�O�i�X�v���C�g�̎��
	enum class FSpriteType
	{
		Shaft,
		LStick,
		Abutton,
		AbuttonPush,
		Clear,
		Faild,

		MaxForeSprite
	};

	//�Q�[�����ł̃V�[�����
	static SceneMode scene;

	//�I�u�W�F�N�g


	//�w�i�X�v���C�g
	std::unique_ptr<NSprite> backSprite_;	//�w�i�X�v���C�g

	//�O�i�X�v���C�g
	std::vector<std::unique_ptr<NSprite>> foreSprite_{ (uint32_t)FSpriteType::MaxForeSprite };		//L�X�e�B�b�NUI
	float slidePos_ = 0.0f;							//���ɑ|���Ă����̃X���C�h�ʒu
	NEasing::EaseTimer slideTimer_ = 0.1f;			//�X���C�h���̃C�[�W���O�p
	NEasing::EaseTimer flashingTimer_ = 1.0f;		//�_�Ń^�C�}�[

	//���C�g����
	std::unique_ptr<NLightGroup> lightGroup_;

	SimpleParticle clearParticle_;					//�N���A���ɏo��p�[�e�B�N��
	NEasing::EaseTimer clearParticleTimer_ = 1.0f;	//�N���A���ɏo��p�[�e�B�N���p�^�C�}�[

public:
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

	//�v���C�����ǂ����擾
	static bool GetIsPlay() { return scene == SceneMode::Play; }
};