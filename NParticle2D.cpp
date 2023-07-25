#include "NParticle2D.h"
#include "NMathUtil.h"

Emitter2D::Emitter2D()
{
	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
}

void Emitter2D::Init()
{
}

void Emitter2D::Update(bool isGravity)
{
	//�������s�����p�[�e�B�N����S�폜
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].frame >= particles_[i].num_frame)
		{
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//�S�p�[�e�B�N���X�V
	for (size_t i = 0; i < particles_.size(); i++)
	{
		particles_[i].timer.Update();

		//�o�߃t���[�����J�E���g
		particles_[i].frame++;

		//�X�P�[���̐��`���
		particles_[i].scale.x = NEasing::lerp(particles_[i].startScale.x, particles_[i].endScale.x, particles_[i].timer.GetTimeRate());
		particles_[i].scale.y = NEasing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].timer.GetTimeRate());

		//�����x�𑬓x�ɉ��Z
		particles_[i].velo += particles_[i].accel;

		//�d�͉��Z
		if (isGravity)
		{
			particles_[i].velo.y += particles_[i].gravity;
		}

		//���x�ɂ��ړ�
		particles_[i].pos += particles_[i].velo;
	}
}

void Emitter2D::DrawBox()
{
	for (auto& p : particles_)
	{
		//SpDS::DrawBox(p.pos.x, p.pos.y, p.scale.x, p.scale.y, p.rot, p.color);
	}
}

void Emitter2D::DrawGraph()
{
	for (auto& p : particles_)
	{
		//SpDS::DrawRotaGraph((uint32_t)p.pos.x, (uint32_t)p.pos.y, p.scale.x, p.scale.y, p.rot, p.key, Anchor::Center, p.color);
	}
}

void Emitter2D::Add(uint32_t addNum, float life, float minScale, float maxScale, NVector2 minVelo, NVector2 maxVelo, NVector2 accel, float minRot, float maxRot, NColor color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//�w�肵���ő吔�����Ă��琶�����Ȃ�
		if (particles_.size() >= maxCount_)
		{
			return;
		}

		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle2D& p = particles_.back();
		//�G�~�b�^�[�̒����烉���_���ō��W������
		float x = (float)MathUtil::Random((uint32_t)-scale_.x, (uint32_t)scale_.x);
		float y = (float)MathUtil::Random((uint32_t)-scale_.y, (uint32_t)scale_.y);
		NVector2 randomPos(x, y);
		//�����͈̔͂���傫�������_���Ō���
		float scale_ = (float)MathUtil::Random((uint32_t)minScale, (uint32_t)maxScale);
		NVector2 randomScale(scale_, scale_);
		//�����͈̔͂����΂����������_���Ō���
		NVector2 velo = { 
			(float)MathUtil::Random((uint32_t)minVelo.x,(uint32_t)maxVelo.x),
			(float)MathUtil::Random((uint32_t)minVelo.y,(uint32_t)maxVelo.y) };
		//�����͈̔͂����]�������_���Ō���
		float rot = (float)MathUtil::Random((uint32_t)minRot, (uint32_t)maxRot);

		//���܂������W�ɃG�~�b�^�[���̂̍��W�𑫂��Đ������ʒu��
		p.pos = randomPos + pos_;
		//���ł������ɍ��킹�ĉ�]
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.num_frame = (uint32_t)life;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0 };
		p.color = color;
		//�C�[�W���O�p�̃^�C�}�[��ݒ�A�J�n
		p.timer.maxTime_ = life;
		p.timer.Start();
	}
}

void Emitter2D::SetScale(NVector2& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//�g�k�p�Ɍ��̃T�C�Y��ۊ�
}

void Emitter2D::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void Emitter2D::StartScalingTimer(bool isRun)
{
	if (isRun)
	{
		scalingTimer_.Start();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
	else
	{
		scalingTimer_.ReverseStart();	//�ݒ�Ɠ����Ƀ^�C�}�[���X�^�[�g
	}
}
