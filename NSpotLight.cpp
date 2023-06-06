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
	//値の更新があったときだけ定数バッファに転送する
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NSpotLight::Draw(UINT rootParameterIndex)
{
	//定数バッファビューをセット
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbSpotLight->constBuff->GetGPUVirtualAddress());
}

void NSpotLight::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbSpotLight->constMap = nullptr;
	result = cbSpotLight->constBuff->Map(0, nullptr, (void**)&cbSpotLight->constMap);
	if (SUCCEEDED(result)) {
		cbSpotLight->constMap->dir = -lightdir;	//ライトの向きは逆向きで
		cbSpotLight->constMap->color = lightcolor;
		cbSpotLight->constMap->atten = lightatten;
		cbSpotLight->constMap->factorAngleCos = lightFactorAngleCos;
		cbSpotLight->constMap->active = isActive;
		cbSpotLight->constBuff->Unmap(0, nullptr);
	}
}

void NSpotLight::SetLightDir(const NVector3& lightdir)
{
	//正規化してセット
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
