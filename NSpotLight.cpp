#include "NSpotLight.h"
#include "NDX12.h"

NSpotLight::NSpotLight()
{
}

NSpotLight::~NSpotLight()
{
}

void NSpotLight::Initialize()
{
	cbSpotLight = std::make_unique<NConstBuff<ConstBuffDataSpotLight>>();
	cbSpotLight->Init();

	SetActive(true);
	TransferConstBuffer();
}

void NSpotLight::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if (isDirty_) {
		TransferConstBuffer();
		isDirty_ = false;
	}
}

void NSpotLight::Draw(uint32_t rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbSpotLight->constBuff_->GetGPUVirtualAddress());
}

void NSpotLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbSpotLight->constMap_ = nullptr;
	result = cbSpotLight->constBuff_->Map(0, nullptr, (void**)&cbSpotLight->constMap_);
	if (SUCCEEDED(result)) {
		cbSpotLight->constMap_->dir = -lightdir_;	//���C�g�̌����͋t������
		cbSpotLight->constMap_->color = lightcolor_;
		cbSpotLight->constMap_->atten = lightatten_;
		cbSpotLight->constMap_->factorAngleCos = lightFactorAngleCos_;
		cbSpotLight->constMap_->active = isActive_;
		cbSpotLight->constBuff_->Unmap(0, nullptr);
	}
}

void NSpotLight::SetLightDir(const NVector3& lightdir)
{
	//���K�����ăZ�b�g
	lightdir_ = lightdir.Normalize();
	isDirty_ = true;
}

void NSpotLight::SetLightPos(const NVector3& lightpos)
{
	lightpos_ = lightpos;
	isDirty_ = true;
}

void NSpotLight::SetLightColor(const NVector3& lightcolor)
{
	lightcolor_ = lightcolor;
	isDirty_ = true;
}

void NSpotLight::SetLightAtten(const NVector3& lightatten)
{
	lightatten_ = lightatten;
	isDirty_ = true;
}

void NSpotLight::SetLightFactorAngle(const NVector2& lightFactorAngle)
{
	lightFactorAngleCos_.x = cosf(MathUtil::Degree2Radian(lightFactorAngle.x));
	lightFactorAngleCos_.y = cosf(MathUtil::Degree2Radian(lightFactorAngle.y));
	isDirty_ = true;
}
