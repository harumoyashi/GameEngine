#include "NCircleShadow.h"
#include "NDX12.h"

NCircleShadow::NCircleShadow()
{
}

NCircleShadow::~NCircleShadow()
{
}

void NCircleShadow::Init()
{
	cbCircleShadow_ = std::make_unique<NConstBuff<ConstBuffDataCircleShadow>>();
	cbCircleShadow_->Init();

	SetActive(true);
	SetFactorAngle({ 0.2f,0.5f });
	TransferConstBuffer();
}

void NCircleShadow::Update()
{
	//値の更新があったときだけ定数バッファに転送する
	if (isDirty_) {
		TransferConstBuffer();
		isDirty_ = false;
	}
}

void NCircleShadow::Draw(const uint32_t rootParameterIndex)
{
	if (isActive_)
	{
		//定数バッファビューをセット
		NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
			cbCircleShadow_->constBuff_->GetGPUVirtualAddress());
	}
}

void NCircleShadow::TransferConstBuffer()
{
	HRESULT result;
	// 定数バッファへデータ転送
	cbCircleShadow_->constMap_ = nullptr;
	result = cbCircleShadow_->constBuff_->Map(0, nullptr, (void**)&cbCircleShadow_->constMap_);
	if (SUCCEEDED(result)) {
		cbCircleShadow_->constMap_->dir = -dir_;	//ライトの向きは逆向きで
		cbCircleShadow_->constMap_->pos = casterPos_;
		cbCircleShadow_->constMap_->disCasterLight = distanceCasterLight_;
		cbCircleShadow_->constMap_->atten = atten_;
		cbCircleShadow_->constMap_->factorAngleCos = factorAngleCos_;
		cbCircleShadow_->constMap_->active = isActive_;
		cbCircleShadow_->constBuff_->Unmap(0, nullptr);
	}
}

void NCircleShadow::SetDir(const NVector3& dir)
{
	dir_ = dir.Normalize();
	isDirty_ = true;
}

void NCircleShadow::SetCasterPos(const NVector3& casterPos)
{
	casterPos_ = casterPos;
	isDirty_ = true;
}

void NCircleShadow::SetDistanceCasterLight(const float distanceCasterLight)
{
	distanceCasterLight_ = distanceCasterLight;
	isDirty_ = true;
}

void NCircleShadow::SetAtten(const NVector3& atten)
{
	atten_ = atten;
	isDirty_ = true;
}

void NCircleShadow::SetFactorAngle(const NVector2& factorAngle)
{
	factorAngleCos_.x = cosf(MathUtil::Degree2Radian(factorAngle.x));
	factorAngleCos_.y = cosf(MathUtil::Degree2Radian(factorAngle.y));
	isDirty_ = true;
}
