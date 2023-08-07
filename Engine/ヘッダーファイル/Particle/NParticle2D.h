#pragma once
#include <NVec2.h>
#include <NColor.h>
#include <NTexture.h>
#include <NEasing.h>

class Emitter2D final
{
	//���q1��
	struct Particle2D
	{
		//���W
		NVec2 pos;
		//�傫��
		NVec2 scale;
		NVec2 startScale;	//�J�n���̑傫��
		NVec2 endScale;		//�I�����̑傫��
		//�p�x
		float rot = 0;
		//���x
		NVec2 velo;
		//�����x
		NVec2 accel;
		//�d��
		float gravity = 0.98f;

		//�F
		NColor color;
		//���݃t���[��
		uint32_t frame = 0;
		//�I���t���[��
		uint32_t num_frame = 0;

		//�C�[�W���O�p�^�C�}�[
		NEasing::EaseTimer timer = 1.0f;

		//����
		float radius = 0;
		float startRadius = 0;	//�J�n���̋���

		//�摜
		TextureHandle key;
	};

private:
	//���W
	NVec2 pos_;
	//�p�x
	float rot_ = 0.0f;
	//�傫��
	NVec2 scale_{ 1.f,1.f };
	float minScale_;	//�G�~�b�^�[�̍ŏ��T�C�Y(�䗦)
	float maxScale_;	//�G�~�b�^�[�̍ő�T�C�Y(�䗦)
	//���̑傫��
	NVec2 originalScale_;
	//�g�k�p�{��
	float scaling_;
	//�g�k�p�^�C�}�[
	NEasing::EaseTimer scalingTimer_ = 1.0f;
	//�p�[�e�B�N���o���^�C�~���O�p�^�C�}�[
	uint32_t particleTimer_ = 0;
	//���t���[���Ɉ��p�[�e�B�N���ǉ����邩
	uint32_t addInterval_;

	//�摜
	TextureHandle key_;

	static const uint32_t maxCount_ = 256;	//�ő吔
	std::vector<Particle2D> particles_;	//�p�[�e�B�N���z��

	bool isActive_ = false;

public:
	Emitter2D();
	//������
	void Init();
	//�X�V
	void Update(bool isGravity = false);
	//�`��
	void DrawBox();
	void DrawGraph();

	//�p�[�e�B�N���ǉ�
	void Add(uint32_t addNum, float life, float minScale, float maxScale, NVec2 minVelo, NVec2 maxVelo,
		NVec2 accel = { 0,0 }, float minRot = 0.0f, float maxRot = 0.0f, NColor color = NColor::kWhite);
	//�p�[�e�B�N���S����
	void ClearParticles() { particles_.clear(); }

	//�Q�b�^�[//
	//���W�擾
	NVec2 GetPos()const { return pos_; }
	//�傫���擾
	NVec2 GetScale()const { return scale_; }

	//�p�[�e�B�N���S�����񂾂��擾
	bool GetParticlesDead()const { return particles_.empty(); }

	//���p�[�e�B�N�����邩�擾
	size_t GetParticlesSize()const { return particles_.size(); }

	//�Z�b�^�[//
	//���W�ݒ�
	void SetPos(float x, float y) { pos_ = { x,y }; }
	void SetPos(NVec2& pos) { pos_ = pos; }
	//�傫���ݒ�
	void SetScale(NVec2& scale);
	//�p�x�ݒ�
	void SetRot(float rot) { rot_ = rot; }

	//�L���t���O�ݒ�
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	//�e�N�X�`���̐ݒ�
	void SetTextureKey(TextureHandle& key) { key_ = key; }

	//�g�k�p�^�C�}�[���؂�ւ�鎞�Ԑݒ�(�b)
	void SetScalingTimer(float timer);
	//�g�k�p�^�C�}�[�J�n
	//isRun:true = �v���X����Ă�,false:�}�C�i�X����Ă�
	void StartScalingTimer(bool isRun = true);
};