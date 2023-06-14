#include "NPointLight.h"
#include "NDX12.h"

NPointLight::NPointLight()
{
}

NPointLight::~NPointLight()
{
}

void NPointLight::Init()
{
	cbPointLight = std::make_unique<NConstBuff<ConstBuffDataPointLight>>();
	cbPointLight->Init();

	SetActive(true);
	TransferConstBuffer();
}

void NPointLight::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if (isDirty_) {
		TransferConstBuffer();
		isDirty_ = false;
	}
}

void NPointLight::Draw(const uint32_t rootParameterIndex)
{
	if (isActive_)
	{
		//�萔�o�b�t�@�r���[���Z�b�g
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
			cbPointLight->constBuff_->GetGPUVirtualAddress());
	}
}

void NPointLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbPointLight->constMap_ = nullptr;
	result = cbPointLight->constBuff_->Map(0, nullptr, (void**)&cbPointLight->constMap_);
	if (SUCCEEDED(result)) {
		cbPointLight->constMap_->pos = lightpos_;
		cbPointLight->constMap_->color = lightcolor_;
		cbPointLight->constMap_->atten = lightatten_;
		cbPointLight->constMap_->active = isActive_;
		cbPointLight->constBuff_->Unmap(0, nullptr);
	}
}

void NPointLight::SetLightPos(const NVector3& lightpos)
{
	lightpos_ = lightpos;
	isDirty_ = true;
}

void NPointLight::SetLightColor(const NVector3& lightcolor)
{
	lightcolor_ = lightcolor;
	isDirty_ = true;
}

void NPointLight::SetLightAtten(const NVector3& lightatten)
{
	lightatten_ = lightatten;
	isDirty_ = true;
}
