#include "SimpleParticle.h"

void SimpleParticle::Init()
{
	IEmitter3D::Init();
	IEmitter3D::SetIsRotation(true);
}

void SimpleParticle::Update()
{
	IEmitter3D::Update();
}

void SimpleParticle::Add(uint32_t addNum, float life, NColor color, float minScale, float maxScale,
	NVec3 minVelo, NVec3 maxVelo, NVec3 accel, NVec3 minRot, NVec3 maxRot)
{
	IEmitter3D::Add(addNum, life, color, minScale, maxScale, minVelo, maxVelo, accel, minRot, maxRot);
}
