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
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NSpotLight::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbSpotLight->constBuff->GetGPUVirtualAddress());
}

void NSpotLight::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbSpotLight->constMap = nullptr;
	result = cbSpotLight->constBuff->Map(0, nullptr, (void**)&cbSpotLight->constMap);
	if (SUCCEEDED(result)) {
		cbSpotLight->constMap->dir = -lightdir;	//���C�g�̌����͋t������
		cbSpotLight->constMap->color = lightcolor;
		cbSpotLight->constMap->atten = lightatten;
		cbSpotLight->constMap->factorAngleCos = lightFactorAngleCos;
		cbSpotLight->constMap->active = isActive;
		cbSpotLight->constBuff->Unmap(0, nullptr);
	}
}

void NSpotLight::SetLightDir(const NVector3& lightdir)
{
	//���K�����ăZ�b�g
	this->lightdir = lightdir.Normalize();
	isDirty = true;
}

void NSpotLight::SetLightPos(const NVector3& lightpos)
{
	this->lightpos = lightpos;
	isDirty = true;
}

void NSpotLight::SetLightColor(const NVector3& lightcolor)
{
	this->lightcolor = lightcolor;
	isDirty = true;
}

void NSpotLight::SetLightAtten(const NVector3& lightatten)
{
	this->lightatten = lightatten;
	isDirty = true;
}

void NSpotLight::SetLightFactorAngle(const NVector2& lightFactorAngle)
{
	this->lightFactorAngleCos.x = cosf(MathUtil::Degree2Radian(lightFactorAngle.x));
	this->lightFactorAngleCos.y = cosf(MathUtil::Degree2Radian(lightFactorAngle.y));
	isDirty = true;
}
