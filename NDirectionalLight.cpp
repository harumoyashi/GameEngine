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
	//値の更新があったときだけ定数バッファに転送する
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NDirectionalLight::Draw(UINT rootParameterIndex)
{
	//定数バッファビューをセット
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbLight->constBuff->GetGPUVirtualAddress());
}

void NDirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbLight->constMap = nullptr;
	result = cbLight->constBuff->Map(0, nullptr, (void**)&cbLight->constMap);
	if (SUCCEEDED(result)) {
		cbLight->constMap->dir = -lightdir;	//ライトの向きは逆向きで
		cbLight->constMap->color = lightcolor;
		cbLight->constBuff->Unmap(0, nullptr);
	}
}

void NDirectionalLight::SetLightDir(const NVector3& lightdir)
{
	//正規化してセット
	this->lightdir = lightdir.Normalize();
	isDirty = true;
}

void NDirectionalLight::SetLightColor(const NVector3& lightcolor)
{
	this->lightcolor = lightcolor;
	isDirty = true;
}