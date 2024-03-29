#include "NDirectionalLight.h"
#include "NDX12.h"

NDirectionalLight::NDirectionalLight()
{
}

NDirectionalLight::~NDirectionalLight()
{
}

void NDirectionalLight::Init()
{
	cbLight = std::make_unique<NConstBuff<ConstBuffDataLight>>();
	cbLight->Init();

	SetActive(true);
	SetLightColor({ 1.0f, 1.0f, 1.0f });
	SetLightDir({ 0.0f, -1.0f, 0.0f });
}

void NDirectionalLight::SetLightDir(const NVec3& lightdir)
{
	//正規化してセット
	lightdir_ = lightdir.Normalize();
	isDirty_ = true;
}

void NDirectionalLight::SetLightColor(const NVec3& lightcolor)
{
	lightcolor_ = lightcolor;
	isDirty_ = true;
}