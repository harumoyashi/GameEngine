#include "NDirectionalLight.h"
#include "NDX12.h"

NDirectionalLight::NDirectionalLight()
{
}

NDirectionalLight::~NDirectionalLight()
{
}

void NDirectionalLight::Initialize()
{
	cbLight = std::make_unique<NConstBuff<ConstBuffDataLight>>();
	cbLight->Init();

	SetActive(true);
	SetLightColor({ 1.0f, 1.0f, 1.0f });
	SetLightDir({ 0.0f, -1.0f, 0.0f });
	

	TransferConstBuffer();
}

void NDirectionalLight::Update()
{
	//値の更新があったときだけ定数バッファに転送する
	if (isDirty_) {
		TransferConstBuffer();
		isDirty_ = false;
	}
}

void NDirectionalLight::Draw(uint32_t rootParameterIndex)
{
	//定数バッファビューをセット
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbLight->constBuff_->GetGPUVirtualAddress());
}

void NDirectionalLight::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbLight->constMap_ = nullptr;
	result = cbLight->constBuff_->Map(0, nullptr, (void**)&cbLight->constMap_);
	if (SUCCEEDED(result)) {
		cbLight->constMap_->dir = -lightdir_;	//ライトの向きは逆向きで
		cbLight->constMap_->color = lightcolor_;
		cbLight->constBuff_->Unmap(0, nullptr);
	}
}

void NDirectionalLight::SetLightDir(const NVector3& lightdir)
{
	//正規化してセット
	lightdir_ = lightdir.Normalize();
	isDirty_ = true;
}

void NDirectionalLight::SetLightColor(const NVector3& lightcolor)
{
	lightcolor_ = lightcolor;
	isDirty_ = true;
}