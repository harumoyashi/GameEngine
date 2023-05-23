#include "NDirectionalLight.h"
#include "NDX12.h"

NDirectionalLight::NDirectionalLight():cbLight(new NConstBuff<ConstBuffDataLight>)
{
}

NDirectionalLight::~NDirectionalLight()
{
	delete cbLight;
}

void NDirectionalLight::Initialize()
{
	cbLight->Init();

	SetActive(true);
	SetLightColor({ 1.0f, 1.0f, 1.0f });
	SetLightDir({ 0.0f, -1.0f, 0.0f });

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

void NDirectionalLight::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbLight->constBuff->GetGPUVirtualAddress());
}

void NDirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbLight->constMap = nullptr;
	result = cbLight->constBuff->Map(0, nullptr, (void**)&cbLight->constMap);
	if (SUCCEEDED(result)) {
		cbLight->constMap->dir = -lightdir;	//���C�g�̌����͋t������
		cbLight->constMap->color = lightcolor;
		cbLight->constBuff->Unmap(0, nullptr);
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