#pragma once
#include <NVector3.h>
#include <NColor.h>
#include <NTexture.h>
#include <NConstBuff.h>
#include <NVertexBuff.h>
#include <NEasing.h>

class IEmitter3D
{
	//���q1��
	struct Particle3D
	{
		//���W
		NVector3 pos;
		//�傫��
		NVector3 scale;
		NVector3 startScale;	//�J�n���̑傫��
		NVector3 endScale;		//�I�����̑傫��
		//�p�x
		float rot = 0;
		//���x
		NVector3 velo;
		//�����x
		NVector3 accel;
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
	//�萔�o�b�t�@//
	std::unique_ptr<NConstBuff<ConstBuffDataTransform>> cbTrans_;	//3D�ϊ��s��

	std::vector<NVertexParticle>vertices_;		//���_�Q
	NVertexBuff<NVertexParticle> vertexBuff_;	//���_�o�b�t�@

	NMatrix4 matWorld_;	//3D�ϊ��s��

	//���W
	NVector3 pos_;
	//�p�x
	float rot_ = 0.0f;
	//�傫��
	NVector3 scale_{ 1.f,1.f,1.f };
	float minScale_;	//�G�~�b�^�[�̍ŏ��T�C�Y(�䗦)
	float maxScale_;	//�G�~�b�^�[�̍ő�T�C�Y(�䗦)
	//���̑傫��
	NVector3 originalScale_;
	//�g�k�p�{��
	float scaling_;
	//�g�k�p�^�C�}�[
	NEasing::EaseTimer scalingTimer_ = 1.0f;
	//�p�[�e�B�N���o���^�C�~���O�p�^�C�}�[
	uint32_t particleTimer_ = 0;
	//���t���[���Ɉ��p�[�e�B�N���ǉ����邩
	uint32_t addInterval_;

	const uint32_t maxParticle_ = 256;	//�ő吔
	std::vector<Particle3D> particles_;		//�p�[�e�B�N���z��

	bool isActive_ = false;					//�L���ɂ��邩�t���O

	NTexture texture_;						//�e�N�X�`��(�g�����킩���)

public:
	IEmitter3D();
	virtual ~IEmitter3D() = default;

	//������
	void Init();
	//�X�V
	void Update(bool isGravity = false);
	//���ʃO���t�B�b�N�X�R�}���h
	static void CommonBeginDraw();
	//�`��
	void Draw();

	//�p�[�e�B�N���ǉ�(�ŗL�����ɂ�����������override�ŏ㏑������)
	virtual void Add(uint32_t addNum, uint32_t life, float minScale, float maxScale, NVector3 minVelo, NVector3 maxVelo,
		NVector3 accel = { 0,0,0 }, float minRot = 0.0f, float maxRot = 0.0f, NColor color = NColor::kWhite);
	//�p�[�e�B�N���S����
	inline void ClearParticles() { particles_.clear(); }

	//�Q�b�^�[//
	//���W�擾
	inline NVector3 GetPos()const { return pos_; }
	//�傫���擾
	inline NVector3 GetScale()const { return scale_; }

	//�p�[�e�B�N���S�����񂾂��擾
	inline bool GetParticlesDead()const { return particles_.empty(); }

	//���p�[�e�B�N�����邩�擾
	inline size_t GetParticlesSize()const { return particles_.size(); }

	//�Z�b�^�[//
	//���W�ݒ�
	inline void SetPos(float x, float y, float z) { pos_ = { x,y,z }; }
	inline void SetPos(NVector3& pos) { pos_ = pos; }
	//�傫���ݒ�
	void SetScale(NVector3& scale);
	//�p�x�ݒ�
	inline void SetRot(float rot) { rot_ = rot; }

	//�L���t���O�ݒ�
	inline void SetIsActive(bool isActive) { isActive_ = isActive; }

	//�g�k�p�^�C�}�[���؂�ւ�鎞�Ԑݒ�(�b)
	void SetScalingTimer(float timer);
	//�g�k�p�^�C�}�[�J�n
	//isRun:true = �v���X����Ă�,false:�}�C�i�X����Ă�
	void StartScalingTimer(bool isRun = true);
};