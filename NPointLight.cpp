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
