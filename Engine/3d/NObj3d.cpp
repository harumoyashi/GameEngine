#include "NObj3d.h"
#include "NInput.h"
#include "NTexture.h"
#include "NCamera.h"
#include "NMathUtil.h"

NLightGroup* NObj3d::sLightGroup = nullptr;

NObj3d::NObj3d()
{
	Init();
}

NObj3d::~NObj3d()
{
}

bool NObj3d::Init()
{
	//�萔�o�b�t�@
	cbTrans_ = std::make_unique<NConstBuff<ConstBuffDataTransform>>();
	cbMaterial_ = std::make_unique<NConstBuff<ConstBuffDataMaterial>>();
	cbColor_ = std::make_unique<NConstBuff<ConstBuffDataColor>>();
	cbTrans_->Init();
	cbMaterial_->Init();
	cbColor_->Init();

	return true;
}

NObj3d* NObj3d::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	NObj3d* obj3d = new NObj3d();
	if (obj3d == nullptr) {
		return nullptr;
	}

	// ������
	if (!obj3d->Init()) {
		delete obj3d;
		assert(0);
		return nullptr;
	}

	float scale_val = 20;
	obj3d->scale_ = { scale_val ,scale_val ,scale_val };

	return obj3d;
}

void NObj3d::Update()
{
	UpdateMatrix();
	TransferMaterial();
	TransferColor();
}

void NObj3d::MoveKey()
{
	//�����ꂩ�̃L�[���������Ƃ�
	if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
	{
		if (NInput::IsKey(DIK_W)) { position_.y += 0.5f; }
		else if (NInput::IsKey(DIK_S)) { position_.y -= 0.5f; }
		if (NInput::IsKey(DIK_D)) { position_.x += 0.5f; }
		else if (NInput::IsKey(DIK_A)) { position_.x -= 0.5f; }
	}

	if (NInput::IsKey(DIK_Q)) { position_.z += 0.5f; }
	else if (NInput::IsKey(DIK_E)) { position_.z -= 0.5f; }

	if (NInput::IsKey(DIK_U)) { rotation_.y += 0.5f; }
	else if (NInput::IsKey(DIK_I)) { rotation_.y -= 0.5f; }
	if (NInput::IsKey(DIK_J)) { rotation_.x += 0.5f; }
	else if (NInput::IsKey(DIK_K)) { rotation_.x -= 0.5f; }
}

void NObj3d::UpdateMatrix()
{
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
	matWorld_ *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld_ *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent_ != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		matWorld_ *= parent_->matWorld_;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	TransferMatrix();
}

void NObj3d::TransferMatrix()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbTrans_->constMap_ = nullptr;
	result = cbTrans_->constBuff_->Map(0, nullptr, (void**)&cbTrans_->constMap_);

	cbTrans_->constMap_->viewproj = NCamera::sCurrentCamera->GetMatView() * NCamera::sCurrentCamera->GetMatProjection();
	cbTrans_->constMap_->world = matWorld_;
	cbTrans_->constMap_->cameraPos = NCamera::sCurrentCamera->GetPos();

	cbTrans_->Unmap();
}

void NObj3d::TransferColor()
{
	cbColor_->constMap_->color = color_;
}

void NObj3d::TransferMaterial()
{
	cbMaterial_->constMap_->ambient = model_.material.ambient;
	cbMaterial_->constMap_->diffuse = model_.material.diffuse;
	cbMaterial_->constMap_->specular = model_.material.specular;
	cbMaterial_->constMap_->alpha = model_.material.alpha;
}

void NObj3d::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState_.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig_.entity_.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	std::vector<ID3D12DescriptorHeap*> ppHeaps = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps((uint32_t)ppHeaps.size(), ppHeaps.data());
}

void NObj3d::Draw()
{
	SetCBV();
	SetVB(model_.vertexBuff.view_);
	SetIB(model_.indexBuff.view_);
	SetSRVHeap(model_.material.texture.gpuHandle_);
	//���C�g�̕`��
	sLightGroup->Draw();
	DrawCommand((uint32_t)model_.indices.size());
}

void NObj3d::SetSRVHeap()
{
	//�V�F�[�_���\�[�X�r���[(SRV)�q�[�v�̐擪�n���h�����擾(SRV���w���Ă�͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NDX12::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h�����w��̂Ƃ��܂Ői�߂�
	uint32_t incrementSize = NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * (uint32_t)texNum_;
	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void NObj3d::SetSRVHeap(const D3D12_GPU_DESCRIPTOR_HANDLE& gpuHandle)
{
	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^0�Ԃɐݒ�
	if (gpuHandle.ptr==0)
	{
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, NTextureManager::GetInstance()->textureMap_["error"].gpuHandle_);
	}
	else
	{
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(0, gpuHandle);
	}
}

void NObj3d::SetVB(const D3D12_VERTEX_BUFFER_VIEW& vbView)
{
	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
}

void NObj3d::SetIB(const D3D12_INDEX_BUFFER_VIEW& ibView)
{
	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
}

void NObj3d::SetCBV()
{
	//���[�g�p�����[�^1�ԂɃ}�e���A���̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(1, cbMaterial_->constBuff_->GetGPUVirtualAddress());
	//���[�g�p�����[�^2�ԂɐF���̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, cbColor_->constBuff_->GetGPUVirtualAddress());
	//���[�g�p�����[�^3�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(3, cbTrans_->constBuff_->GetGPUVirtualAddress());
}

void NObj3d::DrawCommand(const uint32_t indexSize)
{
	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); //�C���f�b�N�X���g���ĕ`��
}

void NObj3d::SetModel(const std::string& modelname)
{
	model_ = NModelManager::GetModel(modelname);
}
