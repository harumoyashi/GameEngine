#include "Player.h"
#include "NInput.h"
#include "BulletFactory.h"
#include "SphereCollider.h"
#include "NCollisionManager.h"
#include "NParticleManager.h"
#include "NAudioManager.h"
#include "Field.h"
#include "RadialBlur.h"

#include <functional>
#include "NImGuiManager.h"
#include "imgui.h"

Player::Player()
{
	obj_ = std::make_unique<NObj3d>();
	obj_->SetModel("catWalk");

	//�p�[�e�B�N���G�~�b�^�[���}�l�[�W���[�ɓo�^
	NParticleManager::GetInstance()->AddEmitter(&deadParticle_, "playerDead");
}

Player::~Player()
{
	//�R���C�_�[�}�l�[�W���[����폜
	NCollisionManager::GetInstance()->RemoveCollider(&collider_);
}

Player* Player::GetInstance()
{
	static Player instance;
	return &instance;
}

bool Player::Init()
{
	obj_->position_ = {};
	obj_->scale_ = 0.1f;
	obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�
	obj_->Update();

	isAlive_ = true;
	isGodmode_ = false;
	isDraw_ = true;

	moveSpeed_ = 0.05f;

	godmodeTimer_.Reset();
	godmodeTimer_.maxTime_ = 2.5f;

	isMove_ = true;
	elapseSpeed_ = 0.0f;

	//�e�̃��x������
	lineLevel_ = 1;
	sideLevel_ = 1;
	wideLevel_ = 0;
	roketLevel_ = 0;

	//�R���C�_�[�ݒ�
	collider_.SetCenterPos(obj_->position_);
	collider_.SetRadius(obj_->scale_.x);
	collider_.SetColID("player");
	NCollisionManager::GetInstance()->AddCollider(&collider_);
	collider_.SetOnCollision(std::bind(&Player::OnCollision, this));

	deadEffectTimer_ = 1.5f;	//�X���[�͍l�������ɉ��b��
	deadEffectTimer_.Reset();

	faildEffectTimer_ = 3.0f;
	faildEffectTimer_.Reset();

	return true;
}

void Player::Update()
{
	//�^�C�}�[�X�V
	deadEffectTimer_.Update();

	if (isAlive_)
	{
		Move();
		Shot();
	}

	obj_->Update();
	collider_.Update(obj_.get());

	//OnCollision()�ŌĂԂƁA���̃t���[���ł̑�������ɉe�����o�邩�炱���ŏ����Ă�
	if (isAlive_ == false)
	{
		if (deadEffectTimer_.GetStarted() == false)
		{
			deadEffectTimer_.Start();
		}

		if (deadEffectTimer_.GetTimeRate() <= 0.2f)	//���S���o��2���̓q�b�g�X�g�b�v�Ɏg��
		{
			isFlashing_ = ((int)(deadEffectTimer_.GetTimeRate() * 100.0f) % 2 == 0);
			if (isFlashing_ ^ obj_->color_ == NColor::kWhite)
			{
				obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�
			}
			else
			{
				obj_->color_ = NColor::kWhite;
			}
			elapseSpeed_ = 0.0f;
		}
		else
		{
			obj_->color_.SetColor255(240, 30, 20, 255);	//�I�����W���ۂ�
			DeadParticle();
			elapseSpeed_ = slowElapseTime_;			//�q�b�g�X�g�b�v�I�������X���[��
			isDraw_ = false;						//�ʏ�͎��񂾂�`�悵�Ȃ�
		}

		//�R���C�_�[�}�l�[�W���[����폜
		NCollisionManager::GetInstance()->RemoveCollider(&collider_);

		deadPos_ = GetPos();	//���U���g�p�Ɏ��񂾍��W���L�^
	}

	//���S���̃p�[�e�B�N�����o�Ă��Ȃ��̂ł���΃|�X�g�G�t�F�N�g�͂����Ȃ�
	if (deadParticle_.GetParticlesDead())
	{
		NPostEffect::SetIsActive(false);
	}
}

void Player::ClearUpdate()
{
	isDraw_ = true;						//��Ε`�悳����
	NPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g�͐؂�
	obj_->rotation_.y = 0.0f;			//�O�Ɍ�������
	obj_->position_.z += 0.05f;			//�O�Ɍ������đ��葱����

	obj_->Update();
}

void Player::FaildUpdate()
{
	//�^�C�}�[���[�v
	faildEffectTimer_.Roop();

	isDraw_ = true;						//��Ε`�悳����
	NPostEffect::SetIsActive(false);	//�|�X�g�G�t�F�N�g�͐؂�
	obj_->position_ = deadPos_ + NVec3(3.0f, 2.0f, -8.0f);	//���񂾍��W����ɓK���Ȓl�����Ă�����ۂ�����

	//���̏�ŉ�]������
	obj_->rotation_.y = MathUtil::Radian2Degree(faildEffectTimer_.GetTimeRate() * PI2);

	obj_->Update();
}

void Player::Draw()
{
	if (isDraw_)
	{
		obj_->SetBlendMode(BlendMode::None);
		obj_->Draw();
		obj_->SetBlendMode(BlendMode::None);
	}
}

void Player::Move()
{
	if (isMove_)
	{
		//�p�b�h�ڑ�����Ă�Ȃ�
		if (NInput::GetIsConnect())
		{
			//�X�e�B�b�N�ړ�
			moveVelo_ = NInput::GetStick();
			//�C�[�W���O�I�Ȋ����ɂ��đ��x�̉ϖ��m��
			moveVelo_.x *= moveVelo_.x * NInput::GetStick().x;
			moveVelo_.y *= moveVelo_.y * NInput::GetStick().y;
		}
		else
		{
			moveVelo_ = { 0,0 };
			//�����ꂩ�̃L�[���������Ƃ�
			if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
			{
				if (NInput::IsKey(DIK_W)) { moveVelo_.y = +1.0f; }
				else if (NInput::IsKey(DIK_S)) { moveVelo_.y = -1.0f; }
				if (NInput::IsKey(DIK_D)) { moveVelo_.x = +1.0f; }
				else if (NInput::IsKey(DIK_A)) { moveVelo_.x = -1.0f; }
			}
		}

		elapseSpeed_ = abs(moveVelo_.x) + abs(moveVelo_.y);	//�ړ��ʂɂ���Čo�ߎ��ԕω�
		elapseSpeed_ = MathUtil::Clamp(elapseSpeed_, 0.0f, 1.0f);

		//�ړ��ʂ����Z
		obj_->position_.x += moveVelo_.x * moveSpeed_;
		obj_->position_.z += moveVelo_.y * moveSpeed_;
		//���Z��ɍs���͈͒����Ă�ꍇ�͒����Ȃ��悤�ɂ���
		obj_->position_.x = (std::max)(obj_->position_.x, -Field::GetInstance()->GetActivityAreaX() + obj_->scale_.x);
		obj_->position_.x = (std::min)(obj_->position_.x, Field::GetInstance()->GetActivityAreaX() - obj_->scale_.x);

		//�ړ������ɍ��킹�ĉ�]
		if (moveVelo_.Length() > 0.0f)			//���͂���Ă���
		{
			NVec2 velo = moveVelo_;	//moveVelo_�̒l���ς��Ȃ��悤�Ɋi�[
			velo.Normalize();
			moveAngle_ = MathUtil::Radian2Degree(acosf(velo.Dot({ 0,1 })));
			if (velo.x < 0)
			{
				moveAngle_ = -moveAngle_;
			}

			obj_->rotation_.y = moveAngle_;
		}
	}

#ifdef _DEBUG
	/*static int lineLv = 1, sideLv = 1;
	ImGui::Begin("PlayerParameter");
	ImGui::SliderFloat("MoveSpeed", &moveSpeed_, 0.01f, 1.0f);
	ImGui::SliderInt("LineLevel", &lineLv, 0, maxBulLevel_);
	ImGui::SliderInt("SideLevel", &sideLv, 0, maxBulLevel_);
	ImGui::Text("MoveVelo:%f,%f", moveVelo_.x, moveVelo_.y);
	ImGui::Text("MoveLen:%f", moveVelo_.Length());
	ImGui::End();
	lineLevel_ = lineLv;
	sideLevel_ = sideLv;*/
#endif //DEBUG
}

void Player::Shot()
{
	BulletFactory::GetInstance()->Update();

	if (lineLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(BulletType::LineBullet,
				obj_->position_ + NVec3(0, obj_->scale_.y, 0), lineLevel_);
	}

	if (sideLevel_ > 0)
	{
		BulletFactory::GetInstance()->
			Create(BulletType::SideBullet,
				obj_->position_ + NVec3(0, obj_->scale_.y, 0), sideLevel_);
	}
}

void Player::OnCollision()
{
	//�����������肪�G���������̏���
	if (collider_.GetColInfo()->GetColID() == "enemy")
	{
		NAudioManager::Play("deadSE");
		isAlive_ = false;
	}
}

void Player::DeadParticle()
{
	if (isDraw_)
	{
		NAudioManager::Play("explosionSE");
		RadialBlur::Init();		//���W�A���u���[������
		deadParticle_.SetIsRotation(true);
		deadParticle_.SetPos(obj_->position_);
		deadParticle_.Add(
			100, 1.5f, obj_->color_, 0.1f, 1.0f, { -2,-2,-2 }, { 2,2,2 }, NVec3::zero, -NVec3::one, NVec3::one);
	}
}

void Player::LevelUp(BulletType bulletType)
{
	//�w�肳�ꂽ�e�̎�ނ̃��x����1�グ��
	switch (bulletType)
	{
	case BulletType::LineBullet:
		if (lineLevel_ < maxBulLevel_)
		{
			lineLevel_ += 1;
		}
		break;
	case BulletType::SideBullet:
		if (sideLevel_ < maxBulLevel_)
		{
			sideLevel_ += 1;
		}
		break;
	/*case BulletType::WideBullet:
		if (wideLevel_ < maxBulLevel_)
		{
			wideLevel_ += 1;
		}
		break;
	case BulletType::Roket:
		if (wideLevel_ < maxBulLevel_)
		{
			roketLevel_ += 1;
		}
		break;*/
	default:
		break;
	}
}
