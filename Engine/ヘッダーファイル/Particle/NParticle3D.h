#pragma once
#include "NVec3.h"
#include "NColor.h"
#include "NTexture.h"
#include "NLightGroup.h"
#include "NConstBuff.h"
#include "NVertexBuff.h"
#include "NEasing.h"
#include "NTimer.h"
#include "NUtil.h"

class IEmitter3D
{
	//���q1��
	struct Particle3D
	{
		//���W
		NVec3 pos;
		//�傫��
		float scale;
		float startScale;	//�J�n���̑傫��
		float endScale;		//�I�����̑傫��
		//�p�x
		NVec3 rot;
		NVec3 plusRot;	//�X�V�����ŉ�]������Ƃ��Ɏg���p
		//���x
		NVec3 velo;
		//�����x
		NVec3 accel;
		//�d��
		float gravity = 0.98f;

		//�F
		NColor color;
		//��������(�t���[����)
		NEasing::EaseTimer aliveTimer;

		//�C�[�W���O�p�^�C�}�[
		NEasing::EaseTimer easeTimer = 1.0f;

		//�G�~�b�^�[���W����̋���
		float radius = 0;
		float startRadius = 0;	//�J�n���̋���

		//�摜
		//TextureHandle key;
	};

private:
	//�萔�o�b�t�@//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;	//3D�ϊ��s��

	std::vector<NVertexParticle>vertices_;		//���_�Q
	NVertexBuff<NVertexParticle> vertexBuff_;	//���_�o�b�t�@

	NMatrix4 matWorld_;	//3D�ϊ��s��

	// ���C�g
	static NLightGroup* sLightGroup;

	//���W
	NVec3 pos_{};
	//�p�x
	NVec3 rot_{};
	//�傫��
	NVec3 scale_{ 0.1f,0.1f,0.1f };	//����0�ɂ���Ƃ������(�����s��)
	float minScale_;	//�p�[�e�B�N���̍ŏ��T�C�Y
	float maxScale_;	//�p�[�e�B�N���̍ő�T�C�Y
	//���̑傫��
	NVec3 originalScale_;
	//�g�k�p�{��
	float scaling_;
	//�g�k�p�^�C�}�[
	NEasing::EaseTimer scalingTimer_ = 1.0f;
	//�p�[�e�B�N���o���^�C�~���O�p�^�C�}�[
	uint32_t particleTimer_ = 0;
	//���t���[���Ɉ��p�[�e�B�N���ǉ����邩
	uint32_t addInterval_;

	//�o�ߎ��Ԃ̃X�s�[�h(�X���[���[�V�����p)
	float elapseSpeed_;

	const uint32_t maxParticle_ = 25600;	//�ő吔
	std::vector<Particle3D> particles_;		//�p�[�e�B�N���z��

	bool isActive_ = true;					//�L���ɂ��邩�t���O

	bool isGravity_ = false;				//�d�͂̉e���󂯂邩�t���O
	bool isRotation_ = false;				//�񂷂��t���O

	NTexture texture_;						//�e�N�X�`��(�g�����킩���)

public:
	IEmitter3D();
	virtual ~IEmitter3D() = default;

	//������
	//���̂܂܂̏������Ăт����Ȃ�p����ł�����Ă�
	virtual void Init();
	//�X�V
	//���̂܂܂̏������Ăт����Ȃ�p����ł�����Ă�
	virtual void Update();
	//���ʃO���t�B�b�N�X�R�}���h
	static void CommonBeginDraw();
	//�u�����h���[�h�ݒ�
	static void SetBlendMode(BlendMode blendMode);
	//�`��
	void Draw();

	//���[���h�s��̍���
	void UpdateMatrix();
	//�萔�o�b�t�@�֑��M
	void TransferMatrix();

	//�p�[�e�B�N���ǉ�(�ŗL�����ɂ�����������override�ŏ㏑������)
	//life:�b���w��Ȃ̂Œ���
	virtual void Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
		NVec3 minVelo, NVec3 maxVelo, NVec3 accel = {}, NVec3 minRot = {}, NVec3 maxRot = {}) = 0;
	//�p�[�e�B�N���S����
	void ClearParticles() { particles_.clear(); }

	//�Q�b�^�[//
	//���W�擾
	NVec3 GetPos()const { return pos_; }
	//�傫���擾
	NVec3 GetScale()const { return scale_; }
	//�p�[�e�B�N���S�����񂾂��擾
	bool GetParticlesDead()const { return particles_.empty(); }
	//���p�[�e�B�N�����邩�擾
	size_t GetParticlesSize()const { return particles_.size(); }
	//�L���t���O�擾
	bool GetIsActive()const { return isActive_; }

	//�Z�b�^�[//
	//���W�ݒ�
	void SetPos(float x, float y, float z) { pos_ = { x,y,z }; }
	void SetPos(const NVec3& pos) { pos_ = pos; }
	//�傫���ݒ�
	void SetScale(const NVec3& scale);
	//�p�x�ݒ�
	void SetRot(float rot) { rot_ = rot; }
	//���C�g��ݒ�
	static void SetLightGroup(NLightGroup* lightGroup) { sLightGroup = lightGroup; }
	//�L���t���O�ݒ�
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	//�d�̓t���O�ݒ�
	void SetIsGravity(bool isGravity) { isGravity_ = isGravity; }
	//��]�t���O�ݒ�
	void SetIsRotation(bool isRotation) { isRotation_ = isRotation; }

	//�g�k�p�^�C�}�[���؂�ւ�鎞�Ԑݒ�(�b)
	void SetScalingTimer(float timer);
	//�g�k�p�^�C�}�[�J�n
	//isRun:true = �v���X����Ă�,false:�}�C�i�X����Ă�
	void StartScalingTimer(bool isRun = true);

	//�o�ߎ��ԃX�s�[�h�ݒ�
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }
};