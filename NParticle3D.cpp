#include "NParticle3D.h"
#include "NGPipeLine.h"
#include "NCamera.h"
#include "NMathUtil.h"

#include "Player.h"

NLightGroup* IEmitter3D::sLightGroup = nullptr;

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

	//�X�V�����ŃT�C�Y���ς�����Ⴄ����A���炩���ߍő吔�����
	vertices_.resize(maxParticle_);
	//����ɂ���ăo�b�t�@�̏�����������
	vertexBuff_.Init(vertices_);
}

void IEmitter3D::Init()
{
}

void IEmitter3D::Update()
{
	SetElapseSpeed(Player::GetInstance()->GetElapseSpeed());

	//�������s�����p�[�e�B�N����S�폜
	for (size_t i = 0; i < particles_.size(); i++)
	{
		if (particles_[i].aliveTimer.GetisTimeOut())
		{
			particles_.erase(particles_.begin() + i);
			i = (size_t)-1;
		}
	}

	//�S�p�[�e�B�N���X�V
	for (size_t i = 0; i < particles_.size(); i++)
	{
		//�������ԂƃC�[�W���O�p�^�C�}�[�̍X�V
		particles_[i].aliveTimer.Update(elapseSpeed_);
		particles_[i].easeTimer.Update(elapseSpeed_);

		//�X�P�[���̐��`���
		particles_[i].scale = NEasing::lerp(particles_[i].startScale, particles_[i].endScale, particles_[i].easeTimer.GetTimeRate());
		//particles_[i].scale.y = NEasing::lerp(particles_[i].startScale.y, particles_[i].endScale.y, particles_[i].easeTimer.GetTimeRate());

		//�����x�𑬓x�ɉ��Z
		particles_[i].velo += particles_[i].accel;

		//�����̃����_���p�x�����Ƃɉ�
		if (isRotation_)
		{
			particles_[i].rot += particles_[i].plusRot * elapseSpeed_;

			//���]������0�ɖ߂��Ă�����
			if (abs(particles_[i].rot.x) >= PI2)
			{
				particles_[i].rot.x = 0.0f;
			}

			if (abs(particles_[i].rot.y) >= PI2)
			{
				particles_[i].rot.y = 0.0f;
			}

			if (abs(particles_[i].rot.z) >= PI2)
			{
				particles_[i].rot.z = 0.0f;
			}
		}

		//�d�͉��Z
		if (isGravity_)
		{
			particles_[i].velo.y += particles_[i].gravity * elapseSpeed_;
		}

		//���x�ɂ��ړ�
		particles_[i].pos += particles_[i].velo * elapseSpeed_;
	}

	//���_�o�b�t�@�փf�[�^�]��
	//�p�[�e�B�N���̏���1�����f
	for (size_t i = 0; i < particles_.size(); i++)
	{
		NVertexParticle vertex;

		//���W
		vertex.pos = particles_[i].pos;
		//��]
		vertex.rot = particles_[i].rot;
		//�F
		vertex.color = particles_[i].color;
		//�X�P�[��
		vertex.scale = particles_[i].scale;

		vertices_.at(i) = vertex;
	}

	//���񒸓_�����ς��̂ŏ��������Ȃ���
	vertexBuff_.TransferBuffer(vertices_);

	UpdateMatrix();
}

void IEmitter3D::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Particle3d"));
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Particle3d")->pRootSignature);

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST); // �_

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void IEmitter3D::Draw()
{
	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbTrans_->constBuff_->GetGPUVirtualAddress());

	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuff_.GetView());

	//SRV�̐ݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, texture_.gpuHandle_);

	//���C�g�̕`��
	sLightGroup->Draw(3);

	// �`��R�}���h
	NDX12::GetInstance()->GetCommandList()->DrawInstanced((UINT)std::distance(particles_.begin(), particles_.end()), 1, 0, 0);
}

void IEmitter3D::UpdateMatrix()
{
	//���[���h�s��
	NMatrix4 matScale;	//�X�P�[�����O�s��
	matScale = matScale.Scale(scale_);

	NMatrix4 matRot;		//��]�s��
	NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rot_.z));
	NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rot_.x));
	NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rot_.y));
	matRot *= matZ;	//Z������ɉ�]���Ă���
	matRot *= matX;	//X������ɉ�]����
	matRot *= matY;	//Y������ɉ�]

	NMatrix4 matTrans;	//���s�ړ��s��
	matTrans = matTrans.Translation(pos_);

	matWorld_ = matWorld_.Identity();	//�P�ʍs����
	matWorld_ *= matScale;	//���[���h���W�ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld_ *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	// �萔�o�b�t�@�փf�[�^�]��
	TransferMatrix();
}

void IEmitter3D::TransferMatrix()
{
	// �萔�o�b�t�@�փf�[�^�]��
	cbTrans_->constMap_ = nullptr;
	cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();
}

void IEmitter3D::Add(uint32_t addNum, uint32_t life, NColor color, float minScale, float maxScale,
	NVector3 minVelo, NVector3 maxVelo, NVector3 accel, NVector3 minRot, NVector3 maxRot)
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
		float pX = MathUtil::Randomf(-scale_.x, scale_.x);
		float pY = MathUtil::Randomf(-scale_.y, scale_.y);
		float pZ = MathUtil::Randomf(-scale_.z, scale_.z);
		NVector3 randomPos(pX, pY, pZ);
		//�����͈̔͂���傫�������_���Ō���
		float sX = MathUtil::Randomf(minScale, maxScale);
		float sY = MathUtil::Randomf(minScale, maxScale);
		float sZ = MathUtil::Randomf(minScale, maxScale);
		NVector3 randomScale(sX, sY, sZ);
		//�����͈̔͂����΂����������_���Ō���
		float vX = MathUtil::Randomf(minVelo.x, maxVelo.x);
		float vY = MathUtil::Randomf(minVelo.y, maxVelo.y);
		float vZ = MathUtil::Randomf(minVelo.z, maxVelo.z);
		NVector3 randomVelo(vX, vY, vZ);
		//�����͈̔͂����]�������_���Ō���
		float rX = MathUtil::Randomf(minRot.x, maxRot.x);
		float rY = MathUtil::Randomf(minRot.y, maxRot.y);
		float rZ = MathUtil::Randomf(minRot.z, maxRot.z);
		NVector3 randomRot(rX, rY, rZ);

		//���܂������W�ɃG�~�b�^�[���̂̍��W�𑫂��Đ������ʒu��
		p.pos = randomPos + pos_;
		//���ł������ɍ��킹�ĉ�]
		p.rot = randomRot;
		p.plusRot = p.rot;
		p.velo = randomVelo;
		p.accel = accel;
		p.aliveTimer = (float)life;
		p.scale = sX;
		p.startScale = p.scale;
		p.endScale = 0.0f;
		p.color = color;
		//�C�[�W���O�p�̃^�C�}�[��ݒ�A�J�n
		p.easeTimer.maxTime_ = (float)life / 60.0f;
		p.easeTimer.Start();
	}
}

void IEmitter3D::SetScale(const NVector3& scale)
{
	scale_ = scale;
	originalScale_ = scale_;			//�g�k�p�Ɍ��̃T�C�Y��ۊ�
}

void IEmitter3D::SetScalingTimer(float easeTimer)
{
	scalingTimer_.maxTime_ = easeTimer;
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
