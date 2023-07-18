#include "NAssimpModel.h"
#include "NCamera.h"
#include "NGPipeline.h"
#include "NUtil.h"

NLightGroup* NAssimpModel::sLightGroup = nullptr;

void NAssimpModel::Load(const wchar_t* filename)
{
	importSetting_.filename = filename;

	//model�ǂݍ���
	if (!loader_.Load(importSetting_))
	{
		//�Ȃ񂩌x���o�����ق����悳����
	}

	//�e�N�X�`���ǂݍ���
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NTextureManager::GetInstance()->LoadTexture(
			NUtil::ToUTF8(meshes_[i].textureName), NUtil::ToUTF8(meshes_[i].textureName));
	}
}

void NAssimpModel::Init()
{
	// ���b�V���̐��������_�o�b�t�@��p�ӂ���
	vertexBuffers_.reserve(meshes_.size());
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NVertexBuff<NVertexPNU> pVB;
		pVB.Init(meshes_[i].vertices);

		vertexBuffers_.emplace_back(pVB);
	}

	// ���b�V���̐������C���f�b�N�X�o�b�t�@��p�ӂ���
	indexBuffers_.reserve(meshes_.size());
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		NIndexBuff pIB = NIndexBuff(meshes_[i].indices);

		indexBuffers_.emplace_back(pIB);
	}

	//�萔�o�b�t�@������
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();
}

void NAssimpModel::Update()
{
	//3D�ϊ��s����]��
	HRESULT result;

	//���[���h�s��
	NMatrix4 matScale;	//�X�P�[�����O�s��
	matScale = matScale.Scale(scale_);

	NMatrix4 matRot;		//��]�s��
	NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rotation_.z));
	NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rotation_.x));
	NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rotation_.y));
	matRot *= matZ;	//Z������ɉ�]���Ă���
	matRot *= matX;	//X������ɉ�]����
	matRot *= matY;	//Y������ɉ�]

	NMatrix4 matTrans;	//���s�ړ��s��
	matTrans = matTrans.Translation(position_);

	matWorld_ = matWorld_.Identity();	//�P�ʍs����
	matWorld_ *= matScale;	//���[���h���W�ɃX�P�[�����O�𔽉f
	matWorld_ *= matRot;	//���[���h���W�ɉ�]�𔽉f
	matWorld_ *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	// �萔�o�b�t�@�փf�[�^�]��
	cbTrans_->constMap_ = nullptr;
	result = cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();

	//�}�e���A�����]��
	cbMaterial_->constMap_->ambient = material_.ambient;
	cbMaterial_->constMap_->diffuse = material_.diffuse;
	cbMaterial_->constMap_->specular = material_.specular;
	cbMaterial_->constMap_->alpha = material_.alpha;
	//�F���]��
	cbColor_->constMap_->color = color_;
}

void NAssimpModel::Draw()
{
	//���b�V���̐������C���f�b�N�X���̕`����s����������
	for (size_t i = 0; i < meshes_.size(); i++)
	{
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		NDX12::GetInstance()->GetCommandList()->SetPipelineState(NGPipeline::GetState("Obj"));
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(NGPipeline::GetDesc("Obj")->pRootSignature);

		// �v���~�e�B�u�`��̐ݒ�R�}���h
		NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

		std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
		NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());

		//���[�g�p�����[�^1�ԂɃ}�e���A���̒萔�o�b�t�@��n��
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbMaterial_->constBuff_->GetGPUVirtualAddress());
		//���[�g�p�����[�^2�ԂɐF���̒萔�o�b�t�@��n��
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
		//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());

		NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffers_[i].GetView());
		NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(indexBuffers_[i].GetView());

		//�e�N�X�`���̖��O�擾
		std::string texName = NUtil::ToUTF8(meshes_[i].textureName);
		NTexture tex;
		//�����e�N�X�`���Ȃ��Ȃ�^�����ŕ`��
		if (texName == "")
		{
			tex = NTextureManager::GetInstance()->textureMap_["white"];
		}
		else
		{
			tex = NTextureManager::GetInstance()->textureMap_[texName];
		}

		//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^0�Ԃɐݒ�
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, tex.gpuHandle_);

		//���C�g�`��
		sLightGroup->Draw(4);

		NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced((UINT)meshes_[i].indices.size(), 1, 0, 0, 0); // �C���f�b�N�X�̐����`�悷��
	}
}
