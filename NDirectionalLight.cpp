#include "NDirectionalLight.h"
#include "NDX12.h"

NDirectionalLight* NDirectionalLight::Creare()
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	NDirectionalLight* instance = new NDirectionalLight();
	//������
	instance->Initialize();
	//���������C���X�^���X��Ԃ�
	return instance;
}

void NDirectionalLight::Initialize()
{
	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferData) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �萔�o�b�t�@�̐���
	HRESULT result;
	result = NDX12::GetInstance()->GetDevice()->CreateCommittedResource(
		&cbHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	assert(SUCCEEDED(result));

	//�萔�o�b�t�@�փf�[�^�]��
	TransferConstBuffer();
}

void NDirectionalLight::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NDirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex,
		constBuff->GetGPUVirtualAddress());
}

void NDirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->lightv = -lightdir;	//���C�g�̌����͋t������
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void NDirectionalLight::SetLightDir(const NVector3& lightdir)
{
	//���K�����ăZ�b�g
	this->lightdir = lightdir.Normalize();
	isDirty = true;
}

void NDirectionalLight::SetLightColor(const NVector3& lightcolor)
{
	this->lightcolor = lightcolor;
	isDirty = true;
}