#include "NConstBuff.h"
#include "NCamera.h"

void NConstBuff::Init()
{
	SetHeap();
	SetResource();
	Create();
	Mapping();
}

void NConstBuff::SetHeap()
{
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//GPU�ւ̓]���p
}

void NConstBuff::SetResource()
{
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;	//256�o�C�g�A���C�������g
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
}

void NConstBuff::Create()
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

void NConstBuff::Mapping()
{
	HRESULT result;

	result = constBuff->Map(0, nullptr, (void**)&constMapTransform);	//�}�b�s���O
	assert(SUCCEEDED(result));
}

void NConstBuff::TransferMatrix(NMatrix4 matWorld)
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	constMapTransform = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMapTransform);
	
	constMapTransform->viewproj = NCamera::nowCamera->GetMatView() * NCamera::nowCamera->GetMatProjection();
	constMapTransform->world = matWorld;
	constMapTransform->cameraPos = NCamera::nowCamera->GetPos();

	constBuff->Unmap(0, nullptr);
}
