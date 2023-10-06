#include "NLightGroup.h"
#include "NObj3d.h"

std::unique_ptr<NDirectionalLight> NLightGroup::sDirLights[kDirLightNum] = {};
std::unique_ptr<NPointLight> NLightGroup::sPointLights[kPointLightNum] = {};
std::unique_ptr<NSpotLight> NLightGroup::sSpotLights[kSpotLightNum] = {};
std::unique_ptr<NCircleShadow> NLightGroup::sCircleShadows[kCircleShadowNum] = {};

void NLightGroup::Init(bool isDirLights, bool isPointLights,
	bool isSpotLights, bool isCircleShadows)
{
	cbLight_ = std::make_unique<NConstBuff<ConstBuffDataLightGroup>>();
	cbLight_->Init();

	for (uint32_t i = 0; i < kDirLightNum; i++)
	{
		sDirLights[i] = std::make_unique<NDirectionalLight>();
		if (isDirLights)
		{
			sDirLights[i]->Init();
		}
		else
		{
			sDirLights[i]->SetActive(false);
		}
	}

	for (uint32_t i = 0; i < kPointLightNum; i++)
	{
		sPointLights[i] = std::make_unique<NPointLight>();
		if (isPointLights)
		{
			sPointLights[i]->Init();
		}
		else
		{
			sPointLights[i]->SetActive(false);
		}
	}

	for (uint32_t i = 0; i < kSpotLightNum; i++)
	{
		sSpotLights[i] = std::make_unique<NSpotLight>();
		if (isSpotLights)
		{
			sSpotLights[i]->Init();
		}
		else
		{
			sSpotLights[i]->SetActive(false);
		}
	}

	for (uint32_t i = 0; i < kCircleShadowNum; i++)
	{
		sCircleShadows[i] = std::make_unique<NCircleShadow>();
		if (isCircleShadows)
		{
			sCircleShadows[i]->Init();
		}
		else
		{
			sCircleShadows[i]->SetActive(false);
		}
	}

	TransferConstBuffer();
}

void NLightGroup::Update()
{
	//値の更新があった時だけ定数バッファに転送する
	if (isDirty)
	{
		TransferConstBuffer();
		isDirty = false;
	}
}

void NLightGroup::Draw(const uint32_t rootParamIndex)
{
	//定数バッファビューをセット
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(
		(UINT)rootParamIndex,
		cbLight_->constBuff_->GetGPUVirtualAddress());
}

void NLightGroup::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbLight_->constMap_ = nullptr;
	result = cbLight_->constBuff_->Map(0, nullptr, (void**)&cbLight_->constMap_);
	if (SUCCEEDED(result)) {
		cbLight_->constMap_->ambientcolor = ambientcolor;
		for (uint32_t i = 0; i < kDirLightNum; i++)
		{
			if (sDirLights[i]->GetActive())
			{
				cbLight_->constMap_->dirLights[i].active = 1;
				cbLight_->constMap_->dirLights[i].dir = -sDirLights[i]->GetLightDir();
				cbLight_->constMap_->dirLights[i].color = sDirLights[i]->GetLightColor();
			}
			else
			{
				cbLight_->constMap_->dirLights[i].active = 0;
			}
		}

		for (uint32_t i = 0; i < kPointLightNum; i++)
		{
			if (sPointLights[i]->GetActive())
			{
				cbLight_->constMap_->pointLights[i].active = 1;
				cbLight_->constMap_->pointLights[i].pos = sPointLights[i]->GetLightPos();
				cbLight_->constMap_->pointLights[i].color = sPointLights[i]->GetLightColor();
				cbLight_->constMap_->pointLights[i].atten = sPointLights[i]->GetLightAtten();
			}
			else
			{
				cbLight_->constMap_->pointLights[i].active = 0;
			}
		}

		for (uint32_t i = 0; i < kSpotLightNum; i++)
		{
			if (sSpotLights[i]->GetActive())
			{
				cbLight_->constMap_->spotLights[i].active = 1;
				cbLight_->constMap_->spotLights[i].dir = -sSpotLights[i]->GetLightDir();
				cbLight_->constMap_->spotLights[i].pos = sSpotLights[i]->GetLightPos();
				cbLight_->constMap_->spotLights[i].color = sSpotLights[i]->GetLightColor();
				cbLight_->constMap_->spotLights[i].atten = sSpotLights[i]->GetLightAtten();
				cbLight_->constMap_->spotLights[i].factorAngleCos = sSpotLights[i]->GetLightFactorAngle();
			}
			else
			{
				cbLight_->constMap_->spotLights[i].active = 0;
			}
		}

		for (uint32_t i = 0; i < kCircleShadowNum; i++)
		{
			if (sCircleShadows[i]->GetActive())
			{
				cbLight_->constMap_->circleShadows[i].active = 1;
				cbLight_->constMap_->circleShadows[i].dir = -sCircleShadows[i]->GetDir();
				cbLight_->constMap_->circleShadows[i].pos = sCircleShadows[i]->GetCasterPos();
				cbLight_->constMap_->circleShadows[i].disCasterLight = sCircleShadows[i]->GetDistanceCasterLight();
				cbLight_->constMap_->circleShadows[i].atten = sCircleShadows[i]->GetAtten();
				cbLight_->constMap_->circleShadows[i].factorAngleCos = sCircleShadows[i]->GetFactorAngle();
			}
			else
			{
				cbLight_->constMap_->circleShadows[i].active = 0;
			}
		}
		cbLight_->Unmap();
	}
}

void NLightGroup::SetAmbientColor(const NVec3& color)
{
	ambientcolor = color;
	isDirty = true;
}

void NLightGroup::SetDirLightColor(const NVec3& lightcolor)
{
	for (uint32_t i = 0; i < kDirLightNum; i++)
	{
		sDirLights[i]->SetLightColor(lightcolor);
	}
}

void NLightGroup::SetPointLightColor(const NVec3& lightcolor)
{
	for (uint32_t i = 0; i < kPointLightNum; i++)
	{
		sPointLights[i]->SetLightColor(lightcolor);
	}
}

void NLightGroup::SetSpotLightColor(const NVec3& lightcolor)
{
	for (uint32_t i = 0; i < kSpotLightNum; i++)
	{
		sSpotLights[i]->SetLightColor(lightcolor);
	}
}
