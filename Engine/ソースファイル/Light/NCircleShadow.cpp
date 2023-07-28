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
