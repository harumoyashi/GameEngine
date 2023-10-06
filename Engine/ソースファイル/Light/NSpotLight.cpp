#include "NSpotLight.h"
#include "NDX12.h"

NSpotLight::NSpotLight()
{
}

NSpotLight::~NSpotLight()
{
}

void NSpotLight::Init()
{
	cbSpotLight = std::make_unique<NConstBuff<ConstBuffDataSpotLight>>();
	cbSpotLight->Init();

	SetActive(true);
	lightdir_ = { 0,-1,0 };
	lightpos_ = { 0,3,0 };
}

void NSpotLight::SetLightDir(const NVec3& lightdir)
{
	//正規化してセット
	lightdir_ = lightdir.Normalize();
	isDirty_ = true;
}

void NSpotLight::SetLightPos(const NVec3& lightpos)
{
	lightpos_ = lightpos;
	isDirty_ = true;
}

void NSpotLight::SetLightColor(const NVec3& lightcolor)
{
	lightcolor_ = lightcolor;
	isDirty_ = true;
}

void NSpotLight::SetLightAtten(const NVec3& lightatten)
{
	lightatten_ = lightatten;
	isDirty_ = true;
}

void NSpotLight::SetLightFactorAngle(const NVec2& lightFactorAngle)
{
	lightFactorAngleCos_.x = cosf(MathUtil::Degree2Radian(lightFactorAngle.x));
	lightFactorAngleCos_.y = cosf(MathUtil::Degree2Radian(lightFactorAngle.y));
	isDirty_ = true;
}
