#include "NParticle3D.h"
#include "NGPipeLine.h"
#include "NMathUtil.h"

IEmitter3D::IEmitter3D()
{
	//�萔�o�b�t�@
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbTrans_->Init();

	addInterval_ = 0;
	maxScale_ = 0;
	minScale_ = 0;
	scaling_ = 0;
	texture_ = NTextureManager::GetInstance()->textureMap_["white"];	//�Ƃ�ܐ^�����ȃe�N�X�`�����蓖�ĂƂ�

	vertices_.resize(maxParticle_);
	vertexBuff_.Init(vertices_);
}

void IEmitter3D::Init()
{
}

void IEmitter3D::Update(bool isGravity)
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

	//���_�o�b�t�@�փf�[�^�]��
	//�p�[�e�B�N���̏���1�����f
	for (size_t i = 0; i < particles_.size(); i++)
	{
		NVertexParticle vertex;

		//���W
		vertex.pos = particles_[i].pos;
		//�X�P�[��
		vertex.scale = particles_[i].scale;
		//�F
		vertex.color = particles_[i].color;

		vertices_.at(i) = vertex;
	}

	//���񒸓_�����ς��̂ŏ��������Ȃ���
	vertexBuff_.TransferBuffer(vertices_);
}

void IEmitter3D::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle")->pRootSignature);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �_

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void IEmitter3D::Draw()
{
	//���[�g�p�����[�^1�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbTrans_->constBuff_->GetGPUVirtualAddress());

	//SRV�̐ݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture_.gpuHandle_);

	// �`��R�}���h
	NDX12::GetInstance()->GetCommandList()->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()),1,0,0);
}

void IEmitter3D::Add(uint32_t addNum, uint32_t life, float minScale, float maxScale, NVector3 minVelo, NVector3 maxVelo, NVector3 accel, float minRot, float maxRot, NColor color)
{
	for (uint32_t i = 0; i < addNum; i++)
	{
		//�w�肵���ő吔�����Ă��琶�����Ȃ�
		if (particles_.size() >= maxParticle_)
		{
			return;
		}

		//���X�g�ɗv�f��ǉ�
		particles_.emplace_back();
		//�ǉ������v�f�̎Q��
		Particle3D& p = particles_.back();
		//�G�~�b�^�[�̒����烉���_���ō��W������
		float x = (float)MathUtil::Random((uint32_t)-scale_.x, (uint32_t)scale_.x);
		float y = (float)MathUtil::Random((uint32_t)-scale_.y, (uint32_t)scale_.y);
		float z = (float)MathUtil::Random((uint32_t)-scale_.z, (uint32_t)scale_.z);
		NVector3 randomPos(x, y, z);
		//�����͈̔͂���傫�������_���Ō���
		float scale_ = (float)MathUtil::Random((uint32_t)minScale, (uint32_t)maxScale);
		NVector3 randomScale(scale_, scale_, scale_);
		//�����͈̔͂����΂����������_���Ō���
		NVector3 velo = {
			(float)MathUtil::Random((uint32_t)minVelo.x,(uint32_t)maxVelo.x),
			(float)MathUtil::Random((uint32_t)minVelo.y,(uint32_t)maxVelo.y),
			(float)MathUtil::Random((uint32_t)minVelo.z,(uint32_t)maxVelo.z) };
		//�����͈̔͂����]�������_���Ō���
		float rot = (float)MathUtil::Random((uint32_t)minRot, (uint32_t)maxRot);

		//���܂������W�ɃG�~�b�^�[���̂̍��W�𑫂��Đ������ʒu��
		p.pos = randomPos + pos_;
		//���ł������ɍ��킹�ĉ�]
		p.rot = rot;
		p.velo = velo;
		p.accel = accel;
		p.num_frame = life;
		p.scale = randomScale;
		p.startScale = randomScale;
		p.endScale = { 0,0,0 };
		p.color = color;
		//�C�[�W���O�p�̃^�C�}�[��ݒ�A�J�n
		p.timer.maxTime_ = (float)life / 60.0f;
		p.timer.Start();
	}
}

void IEmitter3D::SetScale(NVector3& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//�g�k�p�Ɍ��̃T�C�Y��ۊ�
}

void IEmitter3D::SetScalingTimer(float timer)
{
	scalingTimer_.maxTime_ = timer;
}

void IEmitter3D::StartScalingTimer(bool isRun)
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
