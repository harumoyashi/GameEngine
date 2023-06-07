#include "NPointLight.h"
#include "NDX12.h"

NPointLight::NPointLight()
{
}

NPointLight::~NPointLight()
{
}

void NPointLight::Initialize()
{
	cbPointLight = std::make_unique<NConstBuff<ConstBuffDataPointLight>>();
	cbPointLight->Init();

	SetActive(true);
	TransferConstBuffer();
}

void NPointLight::Update()
{
	//値の更新があったときだけ定数バッファに転送する
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NPointLight::Draw(uint32_t rootParameterIndex)
{
	//定数バッファビューをセット
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbPointLight->constBuff->GetGPUVirtualAddress());
}

void NPointLight::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbPointLight->constMap = nullptr;
	result = cbPointLight->constBuff->Map(0, nullptr, (void**)&cbPointLight->constMap);
	if (SUCCEEDED(result)) {
		cbPointLight->constMap->pos = lightpos;
		cbPointLight->constMap->color = lightcolor;
		cbPointLight->constMap->atten = lightatten;
		cbPointLight->constMap->active = isActive;
		cbPointLight->constBuff->Unmap(0, nullptr);
	}
}

void NPointLight::SetLightPos(const NVector3& lightpos)
{
	this->lightpos = lightpos;
	isDirty = true;
}

void NPointLight::SetLightColor(const NVector3& lightcolor)
{
	this->lightcolor = lightcolor;
	isDirty = true;
}

void NPointLight::SetLightAtten(const NVector3& lightatten)
{
	this->lightatten = lightatten;
	isDirty = true;
}
