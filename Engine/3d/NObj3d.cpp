#include "NObj3d.h"
#include "NInput.h"
#include "NTexture.h"
#include "NCamera.h"
#include "NMathUtil.h"

NLightGroup* NObj3d::lightGroup = nullptr;

NObj3d::~NObj3d()
{
}

bool NObj3d::Init()
{
	//�萔�o�b�t�@
	SetCBHeap();
	SetCBResource();
	CreateCB();
	MappingCB();

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
	obj3d->scale = { scale_val ,scale_val ,scale_val };

	return obj3d;
}

void NObj3d::SetCBHeap()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NObj3d::SetCBResource()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NObj3d::CreateCB()
{
	HRESULT result;

	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProp,	//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff)
	);
	assert(SUCCEEDED(result));
}

void NObj3d::MappingCB()
{
	HRESULT result;

	result = constBuff->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O
	assert(SUCCEEDED(result));
}

void NObj3d::MoveKey()
{
	//�����ꂩ�̃L�[���������Ƃ�
	if (NInput::IsKey(DIK_W) || NInput::IsKey(DIK_S) || NInput::IsKey(DIK_D) || NInput::IsKey(DIK_A))
	{
		if (NInput::IsKey(DIK_W)) { position.y += 0.5f; }
		else if (NInput::IsKey(DIK_S)) { position.y -= 0.5f; }
		if (NInput::IsKey(DIK_D)) { position.x += 0.5f; }
		else if (NInput::IsKey(DIK_A)) { position.x -= 0.5f; }
	}

	if (NInput::IsKey(DIK_Q)) { position.z += 0.5f; }
	else if (NInput::IsKey(DIK_E)) { position.z -= 0.5f; }

	if (NInput::IsKey(DIK_U)) { rotation.y += 0.5f; }
	else if (NInput::IsKey(DIK_I)) { rotation.y -= 0.5f; }
	if (NInput::IsKey(DIK_J)) { rotation.x += 0.5f; }
	else if (NInput::IsKey(DIK_K)) { rotation.x -= 0.5f; }
}

void NObj3d::UpdateMatrix()
{
	HRESULT result;

	//���[���h�s��
	NMatrix4 matScale;	//�X�P�[�����O�s��
	matScale = matScale.Scale(scale);

	NMatrix4 matRot;		//��]�s��
	NMatrix4 matZ = matZ.RotateZ(MathUtil::Degree2Radian(rotation.z));
	NMatrix4 matX = matX.RotateX(MathUtil::Degree2Radian(rotation.x));
	NMatrix4 matY = matY.RotateY(MathUtil::Degree2Radian(rotation.y));
	matRot *= matZ;	//Z������ɉ�]���Ă���
	matRot *= matX;	//X������ɉ�]����
	matRot *= matY;	//Y������ɉ�]

	NMatrix4 matTrans;	//���s�ړ��s��
	matTrans = matTrans.Translation(position);

	matWorld = matWorld.Identity();	//�P�ʍs����
	matWorld *= matScale;	//���[���h���W�ɃX�P�[�����O�𔽉f
	matWorld *= matRot;		//���[���h���W�ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h���W�ɕ��s�ړ��𔽉f

	//�e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		matWorld *= parent->matWorld;
	}

	// �萔�o�b�t�@�փf�[�^�]��
	constMapTransform = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMapTransform);
	TransferMatrix();
	constBuff->Unmap(0, nullptr);
}

void NObj3d::TransferMatrix()
{
	//constMapTransform->mat = matWorld * NCamera::nowCamera->GetMatView() * NCamera::nowCamera->GetMatProjection();
	constMapTransform->viewproj = NCamera::nowCamera->GetMatView() * NCamera::nowCamera->GetMatProjection();
	constMapTransform->world = matWorld;
	constMapTransform->cameraPos = NCamera::nowCamera->GetPos();
}

void NObj3d::CommonBeginDraw()
{
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->SetPipelineState(PipeLineManager::GetInstance()->GetPipelineSet3d().pipelineState.Get());
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootSignature(PipeLineManager::GetInstance()->GetPipelineSet3d().rootSig.entity.Get());

	// �v���~�e�B�u�`��̐ݒ�R�}���h
	NDX12::GetInstance()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g

	ID3D12DescriptorHeap* ppHeaps[] = { NDX12::GetInstance()->GetSRVHeap() };
	NDX12::GetInstance()->GetCommandList()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void NObj3d::Draw()
{

	SetMaterialCBV(model->material);
	SetMatCBV();
	SetVB(model->vertexBuff.view);
	SetIB(model->indexBuff.view);
	SetSRVHeap(model->material.texture.gpuHandle);
	//���C�g�̕`��
	lightGroup->Draw(3);
	DrawCommand((UINT)model->indices.size());
}

void NObj3d::SetMaterialCBV(NMaterial material)
{
	material.SetCBV();
}

void NObj3d::SetSRVHeap()
{
	//�V�F�[�_���\�[�X�r���[(SRV)�q�[�v�̐擪�n���h�����擾(SRV���w���Ă�͂�)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = NDX12::GetInstance()->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart();
	//�n���h�����w��̂Ƃ��܂Ői�߂�
	UINT incrementSize = NDX12::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	srvGpuHandle.ptr += incrementSize * (UINT)texNum;
	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
}

void NObj3d::SetSRVHeap(D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
{
	//�w��̃q�[�v�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(1, gpuHandle);
}

void NObj3d::SetVB(D3D12_VERTEX_BUFFER_VIEW vbView)
{
	NDX12::GetInstance()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
}

void NObj3d::SetIB(D3D12_INDEX_BUFFER_VIEW ibView)
{
	NDX12::GetInstance()->GetCommandList()->IASetIndexBuffer(&ibView);
}

void NObj3d::SetMatCBV()
{
	//���[�g�p�����[�^2�Ԃ�3D�ϊ��s��̒萔�o�b�t�@��n��
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuff->GetGPUVirtualAddress());
}

void NObj3d::DrawCommand(UINT indexSize)
{
	NDX12::GetInstance()->GetCommandList()->DrawIndexedInstanced(indexSize, 1, 0, 0, 0); //�C���f�b�N�X���g���ĕ`��
}
