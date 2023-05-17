#include "NCircleShadow.h"
#include "NDX12.h"

void NCircleShadow::Initialize()
{
	cbCircleShadow.Init();
}

void NCircleShadow::Update()
{
	//�l�̍X�V���������Ƃ������萔�o�b�t�@�ɓ]������
	if (isDirty) {
		TransferConstBuffer();
		isDirty = false;
	}
}

void NCircleShadow::Draw(UINT rootParameterIndex)
{
	//�萔�o�b�t�@�r���[���Z�b�g
	NDX12::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(rootParameterIndex,
		cbCircleShadow.constBuff->GetGPUVirtualAddress());
}

void NCircleShadow::TransferConstBuffer()
{
	HRESULT result;
	// �萔�o�b�t�@�փf�[�^�]��
	cbCircleShadow.constMap = nullptr;
	result = cbCircleShadow.constBuff->Map(0, nullptr, (void**)&cbCircleShadow.constMap);
	if (SUCCEEDED(result)) {
		cbCircleShadow.constMap->dir = -dir;	//���C�g�̌����͋t������
		cbCircleShadow.constMap->pos = casterPos;
		cbCircleShadow.constMap->disCasterLight = distanceCasterLight;
		cbCircleShadow.constMap->atten = atten;
		cbCircleShadow.constMap->factorAngleCos = factorAngleCos;
		cbCircleShadow.constMap->active = isActive;
		cbCircleShadow.constBuff->Unmap(0, nullptr);
	}
}

void NCircleShadow::SetDir(const NVector3& dir)
{
	this->dir = dir.Normalize();
	isDirty = true;
}

void NCircleShadow::SetCasterPos(const NVector3& casterPos)
{
	this->casterPos = casterPos;
	isDirty = true;
}

void NCircleShadow::SetDistanceCasterLight(const float& distanceCasterLight)
{
	this->distanceCasterLight = distanceCasterLight;
	isDirty = true;
}

void NCircleShadow::SetAtten(const NVector3& atten)
{
	this->atten = atten;
	isDirty = true;
}

void NCircleShadow::SetFactorAngle(const NVector2& factorAngle)
{
	this->factorAngleCos.x = cosf(MathUtil::Degree2Radian(factorAngle.x));
	this->factorAngleCos.y = cosf(MathUtil::Degree2Radian(factorAngle.y));
	isDirty = true;
}
