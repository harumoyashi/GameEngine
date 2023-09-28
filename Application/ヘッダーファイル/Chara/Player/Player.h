#pragma once
#include "NObj3d.h"
#include "NCollider.h"
#include "SphereCollider.h"
#include "SimpleParticle.h"
#include "IBullet.h"

class Player final
{
private:
	std::unique_ptr<NObj3d> obj_;
	SphereCollider collider_;	//�v���C���[�̓����蔻��

	bool isAlive_;				//�����Ă邩�t���O
	bool isDraw_;				//�`�悷�邩�t���O

	//------------------------ �ړ��֘A ------------------------//
	bool isMove_;				//�����Ă邩�t���O
	NVec2 moveVelo_;			//�ړ���
	float moveSpeed_;			//�ړ����x

	float moveAngle_;			//��]�p�x

	float elapseSpeed_;			//�o�ߎ���(�������S�Ăɉe�����y�ڂ�)

	//------------------------ �_���[�W�֘A ------------------------//
	bool isGodmode_;			//���G��Ԃ�
	bool isFlashing_;			//�`�J�`�J�����邽�ߗp
	NEasing::EaseTimer godmodeTimer_;		//���G����

	//------------------------ �e�֘A ------------------------//
	const uint32_t maxBulLevel_ = 5;	//�e�̍ō����x��(����)
	uint32_t lineLevel_;				//�ʏ�e�̃��x��
	uint32_t sideLevel_;				//�������ɏo��e�̃��x��
	uint32_t wideLevel_;				//���E�Ɋp�x���ďo��e�̃��x��
	uint32_t roketLevel_;				//���P�b�g�e�̃��x��

	//------------------------ ���񂾂Ƃ��֘A ------------------------//
	SimpleParticle deadParticle_;				//���񂾂Ƃ��ɏo��p�[�e�B�N��
	NEasing::EaseTimer deadEffectTimer_;		//���S���o�ǂ̂��炢�Ő؂�グ�邩�^�C�}�[
	float slowElapseTime_ = 0.01f;				//���񂾂Ƃ��̃X���[���ǂ̂��炢��
	NVec3 deadPos_;								//���񂾍��W�ۑ��p

	//------------------------ ���̑� ------------------------//
	NEasing::EaseTimer faildEffectTimer_;		//���s���o�p�^�C�}�[

	SimpleParticle clearParticle_;					//�N���A���ɏo��p�[�e�B�N��
	NEasing::EaseTimer clearParticleTimer_ = 1.0f;	//�N���A���ɏo��p�[�e�B�N���p�^�C�}�[

public:
	Player();
	~Player();
	static Player* GetInstance();

	//������
	bool Init();
	//�X�V
	void Update();
	//�N���A����p�̍X�V
	void ClearUpdate();
	//���s����p�̍X�V
	void FaildUpdate();
	//�`��
	void Draw();

	//�ړ�����
	void Move();
	//�ˌ�
	void Shot();

	//�����ɓ����������̏���
	void OnCollision();

	//���񂾂Ƃ��̃p�[�e�B�N�����o��
	void DeadParticle();

	//�w�肳�ꂽ�e�̎�ނ̃��x����1�グ��
	void LevelUp(BulletType bulletType);

	// �Q�b�^�[ //
	//���W�擾
	NVec3 GetPos()const { return obj_->position_; }
	//�����_�p�ɂ��傢���߂̍��W�擾
	NVec3 GetHeadPos()const { return { obj_->position_.x,obj_->position_.y + obj_->scale_.y,obj_->position_.z }; }
	//�O�����W�擾
	float GetFrontPosZ()const { return obj_->position_.z + obj_->scale_.z; }
	//�傫���擾
	NVec3 GetScale()const { return obj_->scale_; }
	//���G��Ԏ擾
	bool GetIsGodmode()const { return isGodmode_; }
	//�����Ă邩�t���O�擾
	bool GetIsAlive()const { return isAlive_; }
	//�����邩�t���O�擾
	bool GetIsMove()const { return isMove_; }
	//�R���C�_�[�擾
	const SphereCollider& GetPlayerCollider()const { return collider_; }
	//�ړ��ʎ擾
	NVec2 GetMoveVelo()const { return moveVelo_; }
	//�o�ߎ��Ԏ擾
	float GetElapseSpeed()const { return elapseSpeed_; }
	//�I�u�W�F�N�g�̐F�擾
	NColor& GetColor()const { return obj_->color_; }
	//���S���o���I��������ǂ����擾
	bool GetDeadEffectEnd()const { return deadEffectTimer_.GetEnd(); }

	// �Z�b�^�[ //
	//���W�ݒ�
	void SetPos(const NVec3& pos) { obj_->position_ = pos; }
	//�傫���ݒ�
	void SetScale(const NVec3& scale) { obj_->scale_ = scale; }
	//�A�j���[�V�����Ɍo�ߎ��Ԃ̉e�����󂯂邩�t���O�ݒ�
	void SetIsElapseAnime(bool isElapseAnime) { obj_->SetIsElapseAnime(isElapseAnime); }
	//���G��Ԑݒ�
	void SetIsGodmode(bool isGodmode) { isGodmode_ = isGodmode; }
	//�����Ă邩�t���O�ݒ�
	void SetIsAlive(bool isAlive);
	//�����邩�t���O�ݒ�
	void SetIsMove(bool isMove) { isMove_ = isMove; }
	//�o�ߎ��ԃX�s�[�h�ݒ�
	void SetElapseSpeed(const float elapseSpeed) { elapseSpeed_ = elapseSpeed; }

private:
	Player& operator=(const Player&) = delete;
};